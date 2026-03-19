@Library('cs-shared-lib@1.12.1')
@Library('sx-library-pipeline@4.4.2')

import org.utils.CSSharedLib
import org.utils.Target
import org.utils.OSType
import org.utils.starlinx.SxTarget

def streamwork = new CSSharedLib(
        context: this, 
        projectName: "StreamWork", 
        teamName: "Starlinx", 
        targets: ["starlinx" : [Target.WIN_MSVC2015_X64_QT5_9_6, Target.WIN_MSVC2022_X64_QT5_15]],
        exportControl: "c",
        verbose : true)

def streamworkNoLicense = new CSSharedLib(
        context: this, 
        projectName: "StreamWorkNoLicense", 
        teamName: "Starlinx", 
        targets: ["starlinx" : [Target.WIN_MSVC2015_X64_QT5_9_6, Target.WIN_MSVC2022_X64_QT5_15],
                "STARLINX-LINUX" : [SxTarget.LINUX_GCC11_5_X64_QT5_15]],
        exportControl: "c",
        verbose : true)

Map<CSSharedLib, String> variants = [:]

variants.put(streamwork, "-D LICENSE=ON")
variants.put(streamworkNoLicense, "-D LICENSE=OFF")
pipeline {
    agent {label "starlinx"}
    stages {
        stage ('Starting SwCommon Builder') {
            steps {
                script {
                    variants.each { CSSharedLib key, String cmakeOptions ->
                        def csSharedLib = key as CSSharedLib
                        stage ('Init') {
                            script {
                                csSharedLib.init()
                            }
                        }
                        stage ('CMake configure') {
                            script {
                                csSharedLib.cmakeHelper.configure(options: cmakeOptions, optionsDebug:'-DCMAKE_TOOLCHAIN_FILE=${DEBUG_TOOLCHAIN} -DCMAKE_BUILD_TYPE=Debug' ,optionsRelease:'-DCMAKE_TOOLCHAIN_FILE=${RELEASE_TOOLCHAIN} -DCMAKE_BUILD_TYPE=Release')
                            }
                        }
                        stage ('CMake build') {
                            script {
                                csSharedLib.cmakeHelper.build()
                            }
                        }
                        stage ('CMake Install') {
                            script {
                                csSharedLib.cmakeHelper.install()
                            }
                        }
                        stage ('Create archive') {
                            script {
                                csSharedLib.installerHelper.createArchive()
                            }
                        }
                        stage ('Upload on Nexus and Artifactory') {
                            script {
                                csSharedLib.artefactHelper.upload(subDirectory: "Frameworks")
                            }
                        }
                    }
                }
            }
        }
    }
}
