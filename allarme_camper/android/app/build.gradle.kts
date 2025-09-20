plugins {
    id("com.android.application")
    id("kotlin-android")
    // The Flutter Gradle Plugin must be applied after the Android and Kotlin Gradle plugins.
    id("dev.flutter.flutter-gradle-plugin")
}

android {
    namespace = "com.example.allarme_camper"
    compileSdk = 35 // Consigliato per Flutter 3.x+
    ndkVersion = "27.0.12077973" // <<<<< VERSIONE COMPATIBILE!

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    kotlinOptions {
        jvmTarget = JavaVersion.VERSION_11.toString()
    }

    defaultConfig {
        applicationId = "com.example.allarme_camper"
        minSdk = 21 // oppure flutter.minSdkVersion se lo usi altrove
        targetSdk = 35 // oppure flutter.targetSdkVersion se lo usi altrove
        versionCode = 1 // oppure flutter.versionCode
        versionName = "1.0.0" // oppure flutter.versionName
    }

    buildTypes {
        release {
            // TODO: Add your own signing config for the release build.
            // Signing with the debug keys for now, so `flutter run --release` works.
            signingConfig = signingConfigs.getByName("debug")
        }
    }
}

flutter {
    source = "../.."
}