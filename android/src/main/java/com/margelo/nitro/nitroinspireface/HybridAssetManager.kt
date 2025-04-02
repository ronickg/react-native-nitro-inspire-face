package com.margelo.nitro.nitroinspireface

import com.facebook.proguard.annotations.DoNotStrip
import android.content.Context
import com.margelo.nitro.NitroModules
import java.io.File
import java.io.FileOutputStream
import java.io.IOException

@DoNotStrip
class HybridAssetManager : HybridAssetManagerSpec() {
  override fun getBaseDirectory(): String {
    val context = NitroModules.applicationContext
      ?: throw Error("Application context is unavailable")
    return context.getFilesDir().getAbsolutePath()
  }

  override fun copyAssetToFile(assetPath: String, filePath: String): Boolean {
    val destFile = File(filePath)
    if (destFile.exists()) {
      return true
    }

    val context = NitroModules.applicationContext
      ?: throw Error("Application context is unavailable")

    try {
      context.assets.open(assetPath).use { inputStream ->
        FileOutputStream(filePath).use { outputStream ->
          inputStream.copyTo(outputStream)
        }
      }
      return true
    } catch (e: IOException) {
      throw Error("Asset '$assetPath' does not exist")
    }
  }
}
