package com.margelo.nitro.nitroinspireface

import com.facebook.proguard.annotations.DoNotStrip
import com.margelo.nitro.NitroModules
import com.margelo.nitro.core.Promise
import okhttp3.*
import java.io.File
import java.io.FileInputStream
import java.io.FileOutputStream
import java.io.IOException
import java.security.MessageDigest
import kotlin.coroutines.resume
import kotlin.coroutines.resumeWithException
import kotlin.coroutines.suspendCoroutine

@DoNotStrip
class HybridModelManager : HybridModelManagerSpec() {
    private val context = NitroModules.applicationContext
        ?: throw Error("Application context is unavailable")

    private val cacheDir = File(context.filesDir, "InspireFaceModels").apply {
        if (!exists()) mkdirs()
    }

    private val okHttpClient = OkHttpClient.Builder()
        .connectTimeout(60, java.util.concurrent.TimeUnit.SECONDS)
        .readTimeout(60, java.util.concurrent.TimeUnit.SECONDS)
        .writeTimeout(60, java.util.concurrent.TimeUnit.SECONDS)
        .build()

    override fun downloadModel(
        url: String,
        checksum: String
    ): Promise<String> = Promise.async {
        suspendCoroutine { continuation ->
        println("[HybridModelManager] Downloading model from: $url")

        val request = Request.Builder().url(url).build()
        okHttpClient.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                continuation.resumeWithException(
                    Error("NETWORK_ERROR: ${e.message}")
                )
            }

            override fun onResponse(call: Call, response: Response) {
                if (!response.isSuccessful) {
                    continuation.resumeWithException(
                        Error("NETWORK_ERROR: HTTP ${response.code}")
                    )
                    return
                }

                try {
                    val tempFile = File.createTempFile("model_", ".tar", context.cacheDir)

                    response.body?.let { body ->
                        FileOutputStream(tempFile).use { outputStream ->
                            body.byteStream().use { inputStream ->
                                inputStream.copyTo(outputStream)
                            }
                        }
                    } ?: run {
                        tempFile.delete()
                        continuation.resumeWithException(Error("NETWORK_ERROR: Empty response body"))
                        return
                    }

                    println("[HybridModelManager] Verifying checksum...")
                    val actualChecksum = sha256(tempFile.absolutePath)

                    if (actualChecksum.lowercase() != checksum.lowercase()) {
                        tempFile.delete()
                        continuation.resumeWithException(
                            Error("CHECKSUM_MISMATCH: Expected $checksum, got $actualChecksum")
                        )
                        return
                    }

                    val cacheFile = File(cacheDir, "${checksum.lowercase()}.tar")
                    if (cacheFile.exists()) {
                        cacheFile.delete()
                    }

                    if (!tempFile.renameTo(cacheFile)) {
                        // Fallback to copy if rename fails
                        tempFile.copyTo(cacheFile, overwrite = true)
                        tempFile.delete()
                    }

                    println("[HybridModelManager] Model cached successfully at: ${cacheFile.absolutePath}")
                    continuation.resume(cacheFile.absolutePath)

                } catch (e: Exception) {
                    continuation.resumeWithException(
                        Error("DOWNLOAD_ERROR: ${e.message}")
                    )
                }
            }
        })
        }
    }

    override fun getCachedModel(checksum: String): String? {
        val cachedFile = File(cacheDir, "${checksum.lowercase()}.tar")

        if (!cachedFile.exists()) {
            return null
        }

        return try {
            val actualChecksum = sha256(cachedFile.absolutePath)
            if (actualChecksum.lowercase() == checksum.lowercase()) {
                println("[HybridModelManager] Cache hit for checksum: $checksum")
                cachedFile.absolutePath
            } else {
                println("[HybridModelManager] Cache checksum mismatch, deleting corrupted file")
                cachedFile.delete()
                null
            }
        } catch (e: Exception) {
            println("[HybridModelManager] Error verifying cached model: ${e.message}")
            cachedFile.delete()
            null
        }
    }

    private fun sha256(filePath: String): String {
        val digest = MessageDigest.getInstance("SHA-256")
        FileInputStream(filePath).use { fis ->
            val buffer = ByteArray(8192)
            var read = fis.read(buffer)
            while (read > -1) {
                digest.update(buffer, 0, read)
                read = fis.read(buffer)
            }
        }
        return digest.digest().joinToString("") { "%02x".format(it) }
    }
}
