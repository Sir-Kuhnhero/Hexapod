using System;

using UnityEditor;
using UnityEngine;

using Codice.CM.Common;
using Unity.PlasticSCM.Editor.AssetMenu;
using Unity.PlasticSCM.Editor.AssetsOverlays;
using Unity.PlasticSCM.Editor.AssetsOverlays.Cache;
using Unity.PlasticSCM.Editor.AssetUtils.Processor;
using Unity.PlasticSCM.Editor.CollabMigration;
using Unity.PlasticSCM.Editor.Inspector;
using Unity.PlasticSCM.Editor.ProjectDownloader;
using Unity.PlasticSCM.Editor.SceneView;
using Unity.PlasticSCM.Editor.UI;

namespace Unity.PlasticSCM.Editor
{
    /// <summary>
    /// The Plastic SCM plugin for Unity editor.
    /// </summary>
    [InitializeOnLoad]
    public static class PlasticPlugin
    {
        /// <summary>
        /// Invoked when notification status changed.
        /// </summary>
        public static event Action OnNotificationUpdated = delegate { };

        internal static IAssetStatusCache AssetStatusCache 
        { 
            get { return mAssetStatusCache; } 
        }

        internal static WorkspaceOperationsMonitor WorkspaceOperationsMonitor 
        { 
            get { return mWorkspaceOperationsMonitor; } 
        }

        static PlasticPlugin()
        {
            CloudProjectDownloader.Initialize();
            MigrateCollabProject.Initialize();
            EditorDispatcher.Initialize();

            if (!FindWorkspace.HasWorkspace(ApplicationDataPath.Get()))
                return;

            if (PlasticProjectOfflineMode.IsEnabled())
                return;

            CooldownWindowDelayer cooldownInitializeAction = new CooldownWindowDelayer(
                Enable, UnityConstants.PLUGIN_DELAYED_INITIALIZE_INTERVAL);
            cooldownInitializeAction.Ping();
        }

        /// <summary>
        /// Get the plugin icon.
        /// </summary>
        public static Texture GetPluginIcon()
        {
            return PlasticNotification.GetIcon(mNotificationStatus);
        }

        internal static void Enable()
        {
            if (mIsEnabled)
                return;

            mIsEnabled = true;

            PlasticApp.InitializeIfNeeded();

            if (!FindWorkspace.HasWorkspace(ApplicationDataPath.Get()))
                return;

            EnableForWorkspace();
        }

        internal static void EnableForWorkspace()
        {
            if (mIsEnabledForWorkspace)
                return;

            WorkspaceInfo wkInfo = FindWorkspace.InfoForApplicationPath(
                ApplicationDataPath.Get(), PlasticGui.Plastic.API);

            if (wkInfo == null)
                return;

            mIsEnabledForWorkspace = true;

            PlasticApp.SetWorkspace(wkInfo);

            bool isGluonMode = PlasticGui.Plastic.API.IsGluonWorkspace(wkInfo);

            mAssetStatusCache = new AssetStatusCache(wkInfo, isGluonMode);

            PlasticAssetsProcessor plasticAssetsProcessor = new PlasticAssetsProcessor();

            mWorkspaceOperationsMonitor = BuildWorkspaceOperationsMonitor(
                plasticAssetsProcessor, isGluonMode);
            mWorkspaceOperationsMonitor.Start();

            AssetsProcessors.Enable(
                wkInfo.ClientPath, plasticAssetsProcessor, mAssetStatusCache);
            AssetMenuItems.Enable(
                wkInfo, mAssetStatusCache);
            DrawAssetOverlay.Enable(
                wkInfo.ClientPath, mAssetStatusCache);
            DrawInspectorOperations.Enable(
                wkInfo.ClientPath, mAssetStatusCache);
            DrawSceneOperations.Enable(
                wkInfo.ClientPath, mWorkspaceOperationsMonitor, mAssetStatusCache);
        }

        internal static void Disable()
        {
            try
            {
                PlasticApp.Dispose();

                if (!mIsEnabledForWorkspace)
                    return;

                mWorkspaceOperationsMonitor.Stop();

                AssetsProcessors.Disable();
                AssetMenuItems.Disable();
                DrawAssetOverlay.Disable();
                DrawInspectorOperations.Disable();
                DrawSceneOperations.Disable();
            }
            finally
            {
                mIsEnabled = false;
                mIsEnabledForWorkspace = false;
            }
        }

        internal static void SetNotificationStatus(
            PlasticWindow plasticWindow,
            PlasticNotification.Status status)
        {
            mNotificationStatus = status;

            plasticWindow.SetupWindowTitle(status);

            if (OnNotificationUpdated != null) 
                OnNotificationUpdated.Invoke();
        }

        static WorkspaceOperationsMonitor BuildWorkspaceOperationsMonitor(
            PlasticAssetsProcessor plasticAssetsProcessor,
            bool isGluonMode)
        {
            WorkspaceOperationsMonitor result = new WorkspaceOperationsMonitor(
                PlasticGui.Plastic.API, plasticAssetsProcessor, isGluonMode);
            plasticAssetsProcessor.SetWorkspaceOperationsMonitor(result);
            return result;
        }

        static PlasticNotification.Status mNotificationStatus;
        static AssetStatusCache mAssetStatusCache;
        static WorkspaceOperationsMonitor mWorkspaceOperationsMonitor;
        static bool mIsEnabled;
        static bool mIsEnabledForWorkspace;
    }
}
