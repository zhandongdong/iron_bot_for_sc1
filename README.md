"# iron_bot_for_sc1" 


待修复bug：
iron失去基地时会造成空指针，导致游戏崩溃，
相关代码位置：findxxxbase
特殊情况下，比如遇到和对手换家的局面，会导致输掉比赛


----
README.txt
===============================================================================
                                 Iron - AI for Brood War
===============================================================================

Iron is a StarCraft AI based on the BWEM library (same author).
Iron inherits from Stone its Agent oriented design, and the focus on SCVs and individual behaviors.
Like Stone, it aims at robustness.
Unlike Stone, Iron is an hybrid system: alongside the Multi-Agent paradigm, it uses a bunch of global strategies and algorithms.

Iron always goes for a mech army with lots of vultures.

-------------------------------------------------------------------------------
Installation:

A DLL is provided that can be used directly.

You can compile it at least with Visual Studio 2013 Express, for which a project file is provided.

Windows Environment Variables:
- BWAPI_DIR (as suggested in http://webdocs.cs.ualberta.ca/~cdavid/starcraftaicomp/registration.shtml)

BWAPI version: 4.1.2

-------------------------------------------------------------------------------
License:               MIT/X11

-------------------------------------------------------------------------------
Version:               AIIDE2017

-------------------------------------------------------------------------------
Contact:               igor_dimitrijevic@yahoo.fr

-------------------------------------------------------------------------------
