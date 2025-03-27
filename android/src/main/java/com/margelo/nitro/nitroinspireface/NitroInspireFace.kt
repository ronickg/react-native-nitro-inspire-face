package com.margelo.nitro.nitroinspireface
  
import com.facebook.proguard.annotations.DoNotStrip

@DoNotStrip
class NitroInspireFace : HybridNitroInspireFaceSpec() {
  override fun multiply(a: Double, b: Double): Double {
    return a * b
  }
}
