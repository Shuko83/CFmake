import java.text.SimpleDateFormat

properties([ buildDiscarder(logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '3')) ])

// Branch version
def getBranchVersion()
{
    if (env.BRANCH_NAME.contains('feature/'))
        return env.BRANCH_NAME.substring(env.BRANCH_NAME.lastIndexOf("/") + 1)
    if (env.BRANCH_NAME.contains('version/'))
        return env.BRANCH_NAME.substring(env.BRANCH_NAME.lastIndexOf("/") + 1) + ".0"
    if (env.BRANCH_NAME.contains('release/'))
        return env.BRANCH_NAME.substring(env.BRANCH_NAME.lastIndexOf("/") + 1)
    if (env.BRANCH_NAME.contains('master'))
        return "beta"
    return env.BRANCH_NAME
}


// Artifactory path
def getArtifactoryPath(projectName)
{   
    def version = getBranchVersion()
    echo projectName
    if(env.BRANCH_NAME.contains('release/'))
    {
        return "http://10.20.1.10:8081/artifactory/release/Frameworks/Streamwork/$version/$projectName"
    }
    return "http://10.20.1.10:8081/artifactory/snapshot/Frameworks/Streamwork/beta/$projectName"
}


pipeline {
    agent {label 'starlinx'}

    environment {
        BRANCHVERSION = getBranchVersion()
        COMPILER = "win-msvc2015"
        ARCHITECTURE = "x64"
        QTVERSION = "QT5.9.6"
    }

    stages {
        stage('setup repos') {
            steps {
                script 
		        {
			    	bat label: 'setup repos', script: '''
			    	    ldtools checkout Dependencies.checkout
                        echo racine > .racine
			    	'''
			    }
            }
        }
        stage('generate pro') {
            steps {
                script 
		        {
			    	bat label: 'generate pro', script: '''
                        ldtools pro --generate-dgq
			    	'''
			    }
            }
        }
        stage('creation sln') {
            steps {
                script 
		        {
			    	bat label: 'creation sln', script: '''
                        echo StreamWorkEditor > StreamWork.desc
                        echo SwCore >> StreamWork.desc
                        echo SwEditor >> StreamWork.desc
                        echo SwExecution >> StreamWork.desc
                        echo SwFoundation >> StreamWork.desc
                        echo SwGui >> StreamWork.desc
                        echo SwModel >> StreamWork.desc
                        echo SwServiceConfiguration >> StreamWork.desc
                        echo SwServiceMainWindow >> StreamWork.desc
                        echo SwServiceShortcut >> StreamWork.desc
                        echo SwServiceTools >> StreamWork.desc
                        LDTools sln --exe-to-lib StreamWork.desc
			    	'''
			    }
            }
        }
        stage('compil 64bits') {
            steps {
                script 
		        {
			    	bat label: 'compil', script: '''
                        call "%VS140COMNTOOLS%vcvarsqueryregistry.bat"
                        call "%VC14%vcvarsall.bat" amd64
                        msbuild StreamWork_ExeToLib.sln /p:Configuration="Debug" /t:Rebuild /m /nodeReuse:false
                        msbuild StreamWork_ExeToLib.sln /p:Configuration="Release" /t:Rebuild /m /nodeReuse:false
			    	'''
			    }
            }
        }
        stage('deploy') {
            steps {
                script 
                {
                    for (projet in ["StreamWorkEditor", "SwCore", "SwEditor", "SwExecution","SwFoundation", "SwGui", "SwModel", "SwServiceConfiguration","SwServiceMainWindow" ,"SwServiceShortcut" ,"SwServiceTools"])
                    {
                        env.exportname = projet
                        env.artifactoryPath = getArtifactoryPath(projet)

                        if (env.BRANCH_NAME.contains('master') || env.BRANCH_NAME.contains('release/')) {
                            bat label: 'Deploy', script: '''
                                7z a ".\\%exportname%_%BRANCHVERSION%_%COMPILER%_%ARCHITECTURE%_%QTVERSION%.zip" ".\\Delivery\\%exportname%\\*"
                                curl -uautomate:APBHo6qrEo4eKC9EigYM3w8Lymu -T "%exportname%_%BRANCHVERSION%_%COMPILER%_%ARCHITECTURE%_%QTVERSION%.zip" "%artifactoryPath%/%exportname%_%BRANCHVERSION%_%COMPILER%_%ARCHITECTURE%_%QTVERSION%.zip"
                            '''
                        } else {
                            echo "No deploy on this branch"
                        }
                    }
                }
            }
        }
         stage('End') {
            steps {
                echo 'Ending sucessfully'
            }
        }
    }
    post {         
        always {
            cleanWs()
        }    
    }
}
