using System;
using System.IO;
using System.Collections.Generic;
using UnityEngine;
using NiceIO.Sysroot;
using UnityEditor.Il2Cpp;

namespace UnityEditor.Il2Cpp
{
    /// <summary>
    /// Toolchain for building Linux x86_64 target on Windows x86_64 host
    /// </summary>
    public class ToolchainWindowsX86_64LinuxX86_64: SysrootLinuxX86_64
    {
        private string _packageName           => "com.unity.toolchain.win-x86_64-linux-x86_64";
        /// <summary>
        /// Name of package
        /// </summary>
        public override string Name           => _packageName;
        /// <summary>
        /// Name of host platform
        /// </summary>
        public override string HostPlatform   => "windows";
        /// <summary>
        /// Name of host architecture
        /// </summary>
        public override string HostArch       => "x86_64";
        /// <summary>
        /// Name of target platform
        /// </summary>
        public override string TargetPlatform => "linux";
        /// <summary>
        /// Name of target architecture
        /// </summary>
        public override string TargetArch     => "x86_64";

        private string _payloadVersion => "llvm-9.0.0-1";
        private string _payloadDir;
        private string _target = "x86_64-glibc2.17-linux-gnu";
        private string _linkerFile => "bin/ld.lld";

        private NPath _toolchainPath = null;

        public ToolchainWindowsX86_64LinuxX86_64()
        {
            _payloadDir = $"windows-x86_64-linux-x86_64/{_payloadVersion}";
            RegisterPayload(_packageName, _payloadDir);
            _toolchainPath = PayloadInstallDirectory(_payloadDir);
        }

        /// <summary>
        /// Initialize toolchain
        /// </summary>
        public override bool Initialize()
        {
            UpdatePath();
            return base.Initialize();
        }

        private void UpdatePath()
        {
            string binPath = _toolchainPath.Combine("bin").ToString(SlashMode.Native);
            string paths = Environment.GetEnvironmentVariable("PATH");
            foreach (var path in paths.Split(';'))
            {
                if (path == binPath)
                    return;
            }
            Environment.SetEnvironmentVariable("PATH", $"{paths};{binPath}");
        }

        /// <summary>
        /// Supplies arguments to il2cpp.exe
        /// </summary>
        /// <returns>Next argument to il2cpp.exe</returns>
        public override IEnumerable<string> GetIl2CppArguments()
        {
            var linkerpath = _toolchainPath.Combine(_linkerFile);

            yield return $"--sysroot-path={SysrootInstallDirectory()}";
            yield return $"--compiler-flags=\"-target {_target}\"";
            yield return $"--tool-chain-path={_toolchainPath.InQuotes(SlashMode.Forward)}";
            yield return $"--linker-flags=\"-fuse-ld=\"{linkerpath.InQuotes(SlashMode.Forward)}\" -target {_target} -static-libstdc++\"";
        }

#if !IL2CPP_LEGACY_API_PRESENT
        public override string GetSysrootPath()
        {
            return SysrootInstallDirectory().Trim('"');
        }

        public override string GetToolchainPath()
        {
            return _toolchainPath.ToString();
        }

        public override string GetIl2CppCompilerFlags()
        {
            return $"-target {_target}";
        }

        public override string GetIl2CppLinkerFlags()
        {
            var linkerpath = _toolchainPath.Combine(_linkerFile);

            return $"-fuse-ld={linkerpath.InQuotes(SlashMode.Forward)} -target {_target} -static-libstdc++";
        }
#endif
    }
}
