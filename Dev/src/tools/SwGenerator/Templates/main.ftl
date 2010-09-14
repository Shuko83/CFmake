<#-- main entry -->
<#if doc["/SwFactoryEntry/@IsOwnPluginInDistribution='yes'"]>
	<#-- Plugin generation is on StreamWork distribution (A FAIRE)-->
	<#assign streamwork_src_folder=doc.SwFactoryEntry.@RootRepository>
	<@folder name="${streamwork_src_folder}">
		<#assign prj_folder=doc.SwFactoryEntry.@Name>
		<#-- Project folder -->
		<@folder name="${prj_folder}">
			<#assign filename=doc.SwFactoryEntry.@Name>
			<#assign filename=filename+".pro">
			<@file name="${filename}"><#include "projectdescriptor2.ftl"></@file>
		</@folder>
		<#-- Source folder -->
		<#assign src_folder="${prj_folder}">
		<@folder name="${src_folder}">
			<#assign classename=doc.SwFactoryEntry.@Name>
			<#assign classename=classename+"PluginFactory_Class">
			<#assign filename=classename+".cpp">
			<@file name="${filename}"><#include "factorycppdescriptor.ftl"></@file>
			<#list doc.SwFactoryEntry?children as component>
			<#if component?node_type = 'element'>
			<#if component?node_name = "Components">
			<#assign filename=component.@Name+".cpp">
			<@file name="${filename}"><#include "componentcppdescriptor.ftl"></@file>
			</#if>
			</#if>
			</#list>
		</@folder>
		<#-- Include folder -->
		<#assign inc_folder="${prj_folder}">
		<@folder name="${inc_folder}">
			<#assign filename=doc.SwFactoryEntry.@Name+"Constantes.h">
			<@file name="${filename}"><#include "pluginConstanteshdescriptor.ftl"></@file>
			<#assign classename=doc.SwFactoryEntry.@Name>
			<#assign classename=classename+"PluginFactory_Class">
			<#assign filename=classename+".h">
			<@file name="${filename}"><#include "factoryhdescriptor.ftl"></@file>
			<#list doc.SwFactoryEntry?children as component>
			<#if component?node_type = 'element'>
			<#if component?node_name = "Components">
			<#assign filename=component.@Name+".h">
			<@file name="${filename}"><#include "componenthdescriptor.ftl"></@file>
			</#if>
			</#if>
			</#list>
		</@folder>
		<#assign rscname=doc.SwFactoryEntry.@Name>
			<#assign rscnamefile=rscname+"Rsc.qrc">
			<#-- Resource folder -->
			<@folder name="${prj_folder}_resources">
				<@file name="${rscnamefile}"><#include "rscdescriptor.ftl"></@file>
				<@file name="DefaultContent.txt">empty</@file>
		</@folder>
	</@folder>
<#else>
	<#-- Plugin generation is not on StreamWork distribution (A FAIRE)-->
	<#assign streamwork_src_folder=doc.SwFactoryEntry.@RootRepository>
	<@folder name="${streamwork_src_folder}/Dev/src">
		<#assign prj_folder=doc.SwFactoryEntry.@Name>
		<#-- Project folder -->
		<@folder name="${prj_folder}">
			<#assign filename=doc.SwFactoryEntry.@Name>
			<#assign filename=filename+".pro">
            <@file name="${filename}"><#include "projectdescriptor2.ftl"></@file>
            <@file name="README_FIRST.txt"><#include "readme_first.ftl"></@file>
            <@file name="generatevcproj.bat"><#include "generatevcproj.ftl"></@file>
		</@folder>
		<#-- Source folder -->
		<#assign src_folder="${prj_folder}">
		<@folder name="${src_folder}">
		    <#-- Bodies -->
			<#assign classename=doc.SwFactoryEntry.@Name>
			<#assign classename=classename+"PluginFactory_Class">
			<#assign filename=classename+".cpp">
			<@file name="${filename}"><#include "factorycppdescriptor.ftl"></@file>
			<#list doc.SwFactoryEntry?children as component>
			<#if component?node_type = 'element'>
			<#if component?node_name = "Components">
			<#assign filename=component.@Name+".cpp">
			<@file name="${filename}"><#include "componentcppdescriptor.ftl"></@file>
			</#if>
			</#if>
			</#list>
			<#-- Headers -->
            <#assign filename=doc.SwFactoryEntry.@Name+"Constantes.h">
            <@file name="${filename}"><#include "pluginConstanteshdescriptor.ftl"></@file>
            <#assign classename=doc.SwFactoryEntry.@Name>
            <#assign classename=classename+"PluginFactory_Class">
            <#assign filename=classename+".h">
            <@file name="${filename}"><#include "factoryhdescriptor.ftl"></@file>
            <#list doc.SwFactoryEntry?children as component>
            <#if component?node_type = 'element'>
            <#if component?node_name = "Components">
            <#assign filename=component.@Name+".h">
            <@file name="${filename}"><#include "componenthdescriptor.ftl"></@file>
            </#if>
            </#if>
            </#list>
		</@folder>
		<#-- resource folder -->
		<#assign rscname=doc.SwFactoryEntry.@Name>
			<#assign rscnamefile=rscname+"Rsc.qrc">
			<#-- Resource folder -->
			<@folder name="${prj_folder}/_resources">
				<@file name="${rscnamefile}"><#include "rscdescriptor.ftl"></@file>
				<@file name="DefaultContent.txt">empty</@file>
		</@folder>
	</@folder>
</#if>