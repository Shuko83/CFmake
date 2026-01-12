@Library('cs-shared-lib@1.7.3')
@Library('sx-library-pipeline@4.4.2')

import org.utils.CSSharedLib
import org.utils.Target
import org.utils.OSType
import org.utils.starlinx.SxTarget

def streamwork = new CSSharedLib(
        context: this, 
        projectName: "StreamWork", 
        teamName: "Starlinx", 
        targets: ["starlinx" : [Target.WIN_MSVC2015_X64_QT5_9_6, Target.WIN_MSVC2022_X64_QT5_15],
                "STARLINX-LINUX" : [SxTarget.LINUX_GCC12_2_X64_QT5_15, SxTarget.LINUX_GCC11_5_X64_QT5_15]],
        exportControl: "c",
        verbose : true)

def streamworkNoLicense = new CSSharedLib(
        context: this, 
        projectName: "StreamWorkNoLicense", 
        teamName: "Starlinx", 
        targets: ["starlinx" : [Target.WIN_MSVC2015_X64_QT5_9_6, Target.WIN_MSVC2022_X64_QT5_15],
                "STARLINX-LINUX" : [SxTarget.LINUX_GCC12_2_X64_QT5_15, SxTarget.LINUX_GCC11_5_X64_QT5_15]],
        exportControl: "c",
        verbose : true)

pipeline {
        agent {label "starlinx"}
        stages {
                stage ('Init') {
                steps {
                        script {
                        streamwork.init()
                        streamworkNoLicense.init()
                        }
                }
                }
                stage ('CMake configure') {
                steps {
                        script {
                        streamwork.cmakeHelper.configure(optionsDebug:'-DCMAKE_TOOLCHAIN_FILE=${DEBUG_TOOLCHAIN} -DCMAKE_BUILD_TYPE=Debug' ,optionsRelease:'-DCMAKE_TOOLCHAIN_FILE=${RELEASE_TOOLCHAIN} -DCMAKE_BUILD_TYPE=Release')
                        streamworkNoLicense.cmakeHelper.configure(options: "-DNO_LICENSE=ON", optionsDebug:'-DCMAKE_TOOLCHAIN_FILE=${DEBUG_TOOLCHAIN} -DCMAKE_BUILD_TYPE=Debug' ,optionsRelease:'-DCMAKE_TOOLCHAIN_FILE=${RELEASE_TOOLCHAIN} -DCMAKE_BUILD_TYPE=Release')
                        }
                }
                }
                stage ('CMake build') {
                steps {
                        script {
                        streamwork.cmakeHelper.build()
                        streamworkNoLicense.cmakeHelper.build()
                        }
                }
                }
                stage ('CMake Install') {
                steps {
                        script {
                        streamwork.cmakeHelper.install()
                        streamworkNoLicense.cmakeHelper.install()
                        }
                }
                }
                stage ('Create archive') {
                steps {
                        script {
                        streamwork.installerHelper.createArchive()
                        streamworkNoLicense.installerHelper.createArchive()
                        }
                }
                }
                stage ('Upload on Nexus and Artifactory') {
                steps {
                        script {
                        streamwork.artefactHelper.upload(subDirectory: "Frameworks")
                        streamworkNoLicense.artefactHelper.upload(subDirectory: "Frameworks")
                        }
                }
                }
        }    
}



