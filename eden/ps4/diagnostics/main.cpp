int main() {
    // 1. Cria a pasta raiz do emulador no HD do PS4
    mkdir("/data/EdenPS4", 0777);

    // 2. Injeta o seu arquivo ps4.ini otimizado automaticamente se ele não existir
    const char* iniPath = "/data/EdenPS4/ps4.ini";
    struct stat iniBuffer;
    if (stat(iniPath, &iniBuffer) != 0) {
        FILE* iniFile = fopen(iniPath, "w");
        if (iniFile != NULL) {
            fprintf(iniFile, 
                "[System]\n"
                "docked=0\n"
                "output=720\n"
                "ui_theme=black\n"
                "ui_sounds=on\n"
                "profile=off\n"
                "user=0\n"
                "IgnoreMissingServices=true\n\n"
                "[Controls]\n"
                "player1_type=pro\n"
                "player2_type=pro\n"
                "player3_type=pro\n"
                "player4_type=pro\n\n"
                "[Graphics]\n"
                "renderer=vulkan\n"
                "UseVSync=false\n"
                "UseAsynchronousShaderCompilation=true\n"
                "AnisotropicFiltering=0\n"
                "ResolutionScale=0.40\n" // Sua otimização autoral!
                "LimitSpeedPercent=60\n"
                "ShadowsQuality=0\n"
                "Reflections=false\n"
                "MotionBlur=false\n"
                "Bloom=false\n"
                "AmbientOcclusion=false\n"
                "AntiAliasing=0\n"
                "ForceHighPerformanceGPU=true\n\n"
                "[Memory]\n"
                "inline_memory=1\n"
                "direct_table=1\n"
                "page_reuse=1\n"
                "regalloc=on\n"
                "UnsafeMemoryOptimizations=true\n"
                "DisableGarbageCollection=false\n\n"
                "[Shaders]\n"
                "EnablePipelineCache=true\n"
                "UseDiskShaderCache=true\n"
                "EnableShaderShards=true\n"
                "PrecompileShards=true\n"
                "ShaderCacheSize=2048\n"
            );
            fclose(iniFile);
        }
    }

    log_fd = open("/data/EdenPS4/hardware-ch", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    for (const char* path : {"/mnt/usb0/Eden", "/mnt/usb1/Eden"}) {
        usb_fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (usb_fd >= 0) break;
    }

    // Alterado o nome nos diagnósticos para a sua versão customizada do menu
    Log("\nEden4 Otimizado V1.1 - Diagnostics 00.01 - platform init\n");
    Log("Build: " __DATE__ " " __TIME__ "; user=0x%x\n", sceUserServiceGetForegroundUser);
    Log("Page size=%ld; CPUs=%ld; Pro mode=%d\n", 
        sysconf(_SC_PAGESIZE), 
        sysconf(_SC_NPROCESSORS_ONLN), sceKernelIsPro());
        
    unsigned eax{}, ebx{}, ecx{}, edx{};
    if (__get_cpuid(1, &eax, &ebx, &ecx, &edx))
        Log("CPU feature bits: ecx=%#x, edx=%#x\n", ecx, edx);
        
    const int system = sceSysmoduleLoadModule(0x0001);
    const int user = sceSysmoduleLoadModule(0x0002);
    const int video = sceSysmoduleLoadModule(0x0003);
    Log("Module load: system=%#x user=%#x video=%#x\n", system, user, video);
    
    CheckMemory();
    const bool threads = CheckThreads();
    const bool jit = CheckJit();
    bool display = false;
    
    if (user >= 0 && video >= 0) {
        Log("UserService init: %#x\n", sceUserServiceInitialize(NULL));
        display = ShowResult(threads && jit);
    }
    
    Log("COMPLETE: threads=%s JIT=%s video=%s\n", 
        threads ? "PASS" : "FAIL", 
        jit ? "PASS" : "FAIL", display ? "PASS" : "FAIL");
        
    Log("These checks do not test Eden, Vulkan or graphics.\n");
    if (usb_fd >= 0) close(usb_fd);
    if (log_fd >= 0) close(log_fd);
    if (system >= 0) sceSysmoduleUnloadModule(0x0001);
    return 0;
}

