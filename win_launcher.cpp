#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<windows.h>
#include<Tlhelp32.h>


DWORD pID;
HANDLE hProc = NULL;
uintptr_t baseAddr = 0;

bool isListeningToInput = false;
struct keysAndFunctionCombination {
    char* name;
    char* controlsString;
    int keysPressed = 0;
    bool isToggle = false;
    bool isToggled = false;
    std::vector<int> keys;
    std::map<int,bool> isPressed;
    void (*callback)();
};

std::vector<keysAndFunctionCombination> kafcs;

bool AttachProc(const wchar_t* procName) {
    PROCESSENTRY32 procEntry32;
    procEntry32.dwSize = sizeof(PROCESSENTRY32);
    auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if(hProcSnap == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to make snapshot of process" << std::endl;
        return false;
    }

    while(Process32Next(hProcSnap, &procEntry32)) {
        const char* exeFileName = procEntry32.szExeFile;
        std::string t(exeFileName);
        std::wstring a(t.begin(), t.end());
        if(a.compare(procName) == 0) {
            std::cout << "Found process " << procEntry32.szExeFile << " with procId: " << procEntry32.th32ProcessID << std::endl;
            hProc = OpenProcess(PROCESS_ALL_ACCESS, false, procEntry32.th32ProcessID);
            pID = procEntry32.th32ProcessID;

            if(hProc == NULL) {
                std::cout << "Error opening process." << std::endl;
                return false;
            }

            CloseHandle(hProcSnap);
            return true;
        }
    }
    // printf("Couldn't find '%ls' process.\n", procName);
    return false;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry)) {
            do {
                std::string t(modEntry.szModule);
                std::wstring a(t.begin(), t.end());
                if(a.compare(modName) == 0) {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        } else { std::cout << "Error getting module." << std::endl; }
    } else { std::cout << "Invalid handle value." << std::endl; }
    CloseHandle(hSnap);
    return modBaseAddr;
}

template<class dataType>
void wpm(dataType valToWrite, DWORD addressToWrite) {
    WriteProcessMemory(hProc, (PVOID) addressToWrite, &valToWrite, sizeof(dataType), 0);
}

template<class dataType>
dataType rpm(DWORD addressToRead) {
    dataType rpmBuffer;

    ReadProcessMemory(hProc, (PVOID) addressToRead, &rpmBuffer, sizeof(rpmBuffer), 0);
    return rpmBuffer;
}

template<class dataType>
dataType rptrc(std::vector<DWORD> chain) {
    DWORD addr = baseAddr;
    auto last = std::prev(chain.end(), 1);

    if(chain.size() == 1)
        return rpm<dataType>(addr + *last);

    for(auto v = chain.begin(); v != last ; ++v) {
        addr = rpm<DWORD>(addr + *v);
    }
    
    return rpm<dataType>(addr + *last);
}

void unlockAllDoorsInTheRoom() {
    DWORD lvlFlagsAddr = baseAddr + 0x3F3B6C;
    DWORD lvlFlags = rpm<DWORD>(lvlFlagsAddr);

    for(int i=0; i < 7; ++i) {
        wpm<int>(2, lvlFlags + 0xc + i*4);
    }
}

void teleportPlayerToMouse() {
    std::vector<DWORD> playerPosChain = { 0x2D47EC, 0x64 };
    DWORD playerPos = rptrc<DWORD>(playerPosChain);
    DWORD mousePos = baseAddr + 0x2D4A8C;

    int playerPosX = rpm<int>(playerPos + 0x18);
    int playerPosY = rpm<int>(playerPos + 0x10);
    int playerPosZ = rpm<int>(playerPos + 0x14);

    int mousePosX = rpm<int>(mousePos + 0x8);
    int mousePosY = rpm<int>(mousePos);
    int mousePosZ = rpm<int>(mousePos + 0x4);

    // printf("player: %08x %08x %08x, mouse: %08x %08x %08x",
    //      playerPosX, playerPosY, playerPosZ, mousePosX, mousePosY, mousePosZ);

    wpm<int>(mousePosX, playerPos+0x18);
    wpm<int>(mousePosY, playerPos+0x10);
    wpm<int>(mousePosZ, playerPos+0x14);
}

void vacAllCharsToMouse() {
    DWORD mousePos = baseAddr + 0x2D4A8C;
    std::vector<DWORD> roomCharsChain = { 0x2D47EC, 0x7c };

    DWORD roomChars = rptrc<DWORD>(roomCharsChain);
    
    DWORD numOfChars = rpm<DWORD>(roomChars + 0x36D24);

    int mousePosX = rpm<int>(mousePos + 0x8);
    int mousePosY = rpm<int>(mousePos);
    int mousePosZ = rpm<int>(mousePos + 0x4);

    int maxNumOfChars = numOfChars + 10;
    for(int i=0; i < maxNumOfChars; ++i) {
        DWORD charObj = rpm<DWORD>(roomChars + 0x144 + i*0x1c);
        BYTE charType = rpm<BYTE>(charObj + 0x20);
        DWORD charPos = rpm<DWORD>(charObj + 0x64);

        wpm<int>(mousePosX, charPos + 0x18);
        wpm<int>(mousePosY, charPos + 0x10);
        wpm<int>(mousePosZ, charPos + 0x14);
    }
}

void vacCharsToPlayer(BYTE type) { // type == 0 means all chars
    std::vector<DWORD> playerPosChain = { 0x2D47EC, 0x64 };
    std::vector<DWORD> roomCharsChain = { 0x2D47EC, 0x7c };

    DWORD playerPos = rptrc<DWORD>(playerPosChain);
    DWORD roomChars = rptrc<DWORD>(roomCharsChain);
    
    DWORD numOfChars = rpm<DWORD>(roomChars + 0x36D24);

    int playerPosX = rpm<int>(playerPos + 0x18);
    int playerPosY = rpm<int>(playerPos + 0x10);
    int playerPosZ = rpm<int>(playerPos + 0x14);

    int maxNumOfChars = numOfChars + 10;
    for(int i=0; i < maxNumOfChars; ++i) {
        DWORD charObj = rpm<DWORD>(roomChars + 0x144 + i*0x1c);
        BYTE charType = rpm<BYTE>(charObj + 0x20);
        if(type == 0 || charType == type) {
            DWORD charPos = rpm<DWORD>(charObj + 0x64);

            wpm<int>(playerPosX, charPos + 0x18);
            wpm<int>(playerPosY, charPos + 0x10);
            wpm<int>(playerPosZ, charPos + 0x14);
        }
    }
}

void vacAllCharsToPlayer() {
    vacCharsToPlayer(0);
}

void vacEnemiesToPlayer() {
    vacCharsToPlayer(0x0A);
}

void listenToInput() {
    while(isListeningToInput) {
        for(auto v=kafcs.begin(); v != kafcs.end(); ++v) {
            keysAndFunctionCombination* kafc = &(*v);
            std::vector<int> keys = kafc->keys;
            for(int i=1; i <= 390; ++i) {
                for(auto k=keys.begin(); k != keys.end(); ++k) {
                    if(!kafc->isPressed.count(*k)) {
                        kafc->isPressed.insert(std::make_pair(*k, false));
                        // printf("insert (%d)\n", kafc->isPressed.count(*k));
                    }
                    if(!kafc->isPressed.at(*k) && (GetAsyncKeyState(i) & 0x8000) && *k == i) { // key down
                        kafc->isPressed.at(*k) = true;
                        kafc->keysPressed++; 
                        // printf("%d key held, pressed: %d (%d)\n", i, kafc->keysPressed, kafc->isPressed.at(*k));
                    }
                    else if(kafc->isPressed.at(*k) && !(GetAsyncKeyState(i) & 0x8000)  && *k == i) { //key up
                        kafc->isPressed.at(*k) = false;
                        kafc->keysPressed--; 
                        // printf("%d key released, pressed: %d\n", i, kafc->keysPressed);
                    }
                }
            }
            if(kafc->keysPressed == keys.size()) {
                if(kafc->isToggle) {
                    kafc->isToggled = !kafc->isToggled;
                    printf("%s %s.\n", kafc->name, (kafc->isToggled)? "Activated":"Deactivated");
                } else {
                    printf("%s.\n", kafc->name);
                    kafc->callback();
                }
                Sleep(200);
            }
            if(kafc->isToggle && kafc->isToggled) {
                kafc->callback();
            }
        }
    }
}

void initUnlockAllDoorsInRoom() {
    keysAndFunctionCombination kafc;
    std::vector<int> keys = { VK_CONTROL, 0x44 }; // ctrl+d

    kafc.name = "Unlock All Doors";
    kafc.keys = keys;
    kafc.isToggle = false;
    kafc.callback = unlockAllDoorsInTheRoom;

    kafcs.push_back(kafc);
}

void initTelePlayerToMousePos() {
    keysAndFunctionCombination telePlayerToMouse;
    std::vector<int> keys = { VK_CONTROL, 0x54 }; // ctrl+t

    telePlayerToMouse.name = "Teleport player to mouse position";
    telePlayerToMouse.keys = keys;
    telePlayerToMouse.isToggle = true;
    telePlayerToMouse.callback = teleportPlayerToMouse;

    kafcs.push_back(telePlayerToMouse);
}

void initVacEnemiesToPlayer() {
    keysAndFunctionCombination kafc;
    std::vector<int> keys = { VK_CONTROL, 0x56 }; // ctrl+v

    kafc.name = "Vac enemies to player";
    kafc.keys = keys;
    kafc.isToggle = true;
    kafc.callback = vacEnemiesToPlayer;

    kafcs.push_back(kafc);
}

void initVacAllCharsToMouse() {
    keysAndFunctionCombination kafc;
    std::vector<int> keys = { VK_CONTROL, 0x42 }; // ctrl+b

    kafc.name = "Vac all chars to mouse";
    kafc.keys = keys;
    kafc.isToggle = true;
    kafc.callback = vacAllCharsToMouse;

    kafcs.push_back(kafc);
}

int main(int argc, char* argv[]) {
    const wchar_t* procName = L"silver.exe";

    std::cout << "attaching to process.." << std::endl;
    while(baseAddr == 0) {
        Sleep(1500);
        if(AttachProc(procName)) {
            Sleep(3000);
            baseAddr = GetModuleBaseAddress(pID, procName);
        }
    }
    printf("%08x\n", baseAddr);

    if(baseAddr > 0) {
        initTelePlayerToMousePos();
        initVacEnemiesToPlayer();
        initVacAllCharsToMouse();
        initUnlockAllDoorsInRoom();
    }

    isListeningToInput = true;
    listenToInput();
    //teleportToMouse();

    return 0;
}