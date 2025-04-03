import Foundation
import NitroModules

public class HybridAssetManager: HybridAssetManagerSpec {

    public override init() {
        super.init()
    }

   public func getFilesDirectory() -> String {
        guard let documentsDirectory = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first else {
            fatalError("Application documents directory is unavailable")
        }
        return documentsDirectory.path
    }

    public func getDatabasesDirectory() -> String {
        let fileManager = FileManager.default
        guard let libraryDirectory = fileManager.urls(for: .libraryDirectory, in: .userDomainMask).first else {
            fatalError("Application library directory is unavailable")
        }

        return libraryDirectory.path
    }

    public func copyAssetToFile(assetPath: String, filePath: String) -> Bool {
        let destFile = URL(fileURLWithPath: filePath)

        // Check if file already exists
        if FileManager.default.fileExists(atPath: destFile.path) {
            print("[HybridAssetManager] File already exists at destination")
            return true
        }

        do {
            // Create the destination directory if it doesn't exist
            try FileManager.default.createDirectory(at: destFile.deletingLastPathComponent(),
                                                  withIntermediateDirectories: true)

            // Try to get the asset from the main bundle
            if let assetURL = Bundle.main.url(forResource: assetPath, withExtension: nil) {
                try FileManager.default.copyItem(at: assetURL, to: destFile)
                print("[HybridAssetManager] Successfully copied asset to: \(destFile.path)")
                return true
            } else {
                print("[HybridAssetManager] Failed to find asset in bundle: \(assetPath)")
                throw NSError(domain: "NitroInspireFaceUtils",
                            code: 1,
                            userInfo: [NSLocalizedDescriptionKey: "Asset '\(assetPath)' does not exist"])
            }
        } catch {
            print("[HybridAssetManager] Error copying asset: \(error.localizedDescription)")
            return false
        }
    }
}
