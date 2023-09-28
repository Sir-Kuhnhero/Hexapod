using Unity.PlasticSCM.Editor.UI;

namespace Unity.PlasticSCM.Editor
{
    internal static class PlasticProjectOfflineMode
    {
        internal static bool IsEnabled()
        {
            return BoolSetting.Load(
                UnityConstants.OFFLINE_MODE_ENABLED_KEY_NAME,
                false);
        }

        internal static void Enable()
        {
            BoolSetting.Save(
                true,
                UnityConstants.OFFLINE_MODE_ENABLED_KEY_NAME);
        }

        internal static void Disable()
        {
            BoolSetting.Save(
                false,
                UnityConstants.OFFLINE_MODE_ENABLED_KEY_NAME);
        }
    }
}
