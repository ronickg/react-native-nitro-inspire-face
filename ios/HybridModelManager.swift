import Foundation
import NitroModules
import CryptoKit

public class HybridModelManager: HybridModelManagerSpec {
    private let fileManager = FileManager.default

    // Cache directory in Application Support (persists across updates)
    private lazy var cacheDir: URL = {
        guard let appSupport = fileManager.urls(for: .applicationSupportDirectory, in: .userDomainMask).first else {
            fatalError("Application support directory is unavailable")
        }
        let dir = appSupport.appendingPathComponent("InspireFaceModels")

        if !fileManager.fileExists(atPath: dir.path) {
            try? fileManager.createDirectory(at: dir, withIntermediateDirectories: true, attributes: nil)
        }

        return dir
    }()

    public override init() {
        super.init()
    }

    public func downloadModel(
        url: String,
        checksum: String
    ) throws -> Promise<String> {
        return Promise.async {
            guard let downloadURL = URL(string: url) else {
                throw NSError(domain: "HybridModelManager", code: 1,
                             userInfo: [NSLocalizedDescriptionKey: "INVALID_URL: Malformed URL"])
            }

            let tempDir = FileManager.default.temporaryDirectory
            let tempFile = tempDir.appendingPathComponent(UUID().uuidString + ".tar")

            do {
                print("[HybridModelManager] Downloading model from: \(url)")

                let session = URLSession.shared

                let (tempURL, response) = try await session.download(from: downloadURL)

                if let httpResponse = response as? HTTPURLResponse {
                    guard (200...299).contains(httpResponse.statusCode) else {
                        throw NSError(domain: "HybridModelManager", code: 2,
                                     userInfo: [NSLocalizedDescriptionKey: "NETWORK_ERROR: HTTP \(httpResponse.statusCode)"])
                    }
                }

                try self.fileManager.moveItem(at: tempURL, to: tempFile)

                print("[HybridModelManager] Verifying checksum...")
                let actualChecksum = try self.sha256(filePath: tempFile.path)

                guard actualChecksum.lowercased() == checksum.lowercased() else {
                    try? self.fileManager.removeItem(at: tempFile)
                    throw NSError(domain: "HybridModelManager", code: 3,
                                 userInfo: [NSLocalizedDescriptionKey: "CHECKSUM_MISMATCH: Expected \(checksum), got \(actualChecksum)"])
                }

                let cacheFile = self.cacheDir.appendingPathComponent("\(checksum.lowercased()).tar")

                if self.fileManager.fileExists(atPath: cacheFile.path) {
                    try? self.fileManager.removeItem(at: cacheFile)
                }

                try self.fileManager.moveItem(at: tempFile, to: cacheFile)

                print("[HybridModelManager] Model cached successfully at: \(cacheFile.path)")
                return cacheFile.path

            } catch let error as NSError {
                try? self.fileManager.removeItem(at: tempFile)

                if error.domain == NSURLErrorDomain {
                    throw NSError(domain: "HybridModelManager", code: 2,
                                 userInfo: [NSLocalizedDescriptionKey: "NETWORK_ERROR: \(error.localizedDescription)"])
                }

                throw error
            }
        }
    }

    public func getCachedModel(checksum: String) throws -> String? {
        let cachedFile = cacheDir.appendingPathComponent("\(checksum.lowercased()).tar")

        guard fileManager.fileExists(atPath: cachedFile.path) else {
            return nil
        }

        do {
            let actualChecksum = try sha256(filePath: cachedFile.path)
            if actualChecksum.lowercased() == checksum.lowercased() {
                print("[HybridModelManager] Cache hit for checksum: \(checksum)")
                return cachedFile.path
            } else {
                // Checksum mismatch - delete corrupted file
                print("[HybridModelManager] Cache checksum mismatch, deleting corrupted file")
                try? fileManager.removeItem(at: cachedFile)
                return nil
            }
        } catch {
            print("[HybridModelManager] Error verifying cached model: \(error.localizedDescription)")
            try? fileManager.removeItem(at: cachedFile)
            return nil
        }
    }

    private func sha256(filePath: String) throws -> String {
        let fileURL = URL(fileURLWithPath: filePath)
        let fileData = try Data(contentsOf: fileURL)

        let hash = SHA256.hash(data: fileData)
        return hash.compactMap { String(format: "%02x", $0) }.joined()
    }
}
