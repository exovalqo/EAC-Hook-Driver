# EAC-Hook-Driver
 Kernel driver that will be utilized for logging/hooking EasyAntiCheat(EAC) WinAPI calls
 
 This project is for research purposes only.
  
**What is EasyAntiCheat?:**   
 EasyAntiCheat (or EAC) is a popular anti-cheat that protects a great variety of video games. It is certainly effective in preventing cheating, so reverse engineering the driver (EasyAntiCheat.sys) brings to light many unique and interesting techniques.  

 **Brief Description:**  
 Windows kernel driver that allows hooking of EAC Window API calls.  
 
 **How hooking works:**  
 EasyAntiCheat.sys dynamically resolves Windows API function address. This project changes the address that contains the dynamic address of the Windows API. This results in EAC calling our function.
 
 **Use cases:**  
 Allows us to trace and log parameters of particular Window API functions. Also enables us to spoof return values, which can circumvent Anti Cheat protection checks.  
