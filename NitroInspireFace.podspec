require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))
min_ios_version_supported = '12.0'

Pod::Spec.new do |s|
  s.name         = "NitroInspireFace"
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platforms    = { :ios => min_ios_version_supported }
  s.source       = { :git => "https://github.com/ronickg/react-native-nitro-inspire-face.git", :tag => "#{s.version}" }

  s.source_files = ["ios/**/*.{h,m,mm,swift}", "cpp/**/*.{hpp,cpp}"]

  load 'nitrogen/generated/ios/NitroInspireFace+autolinking.rb'
  add_nitrogen_files(s)

  s.vendored_frameworks = ["ios/MNN.xcframework","ios/InspireFace.xcframework"]
  s.frameworks = 'CoreML'

# Use install_modules_dependencies helper to install the dependencies if React Native version >=0.71.0.
# See https://github.com/facebook/react-native/blob/febf6b7f33fdb4904669f99d795eba4c0f95d7bf/scripts/cocoapods/new_architecture.rb#L79.
if respond_to?(:install_modules_dependencies, true)
  install_modules_dependencies(s)
else
  s.dependency "React-Core"
end
end
