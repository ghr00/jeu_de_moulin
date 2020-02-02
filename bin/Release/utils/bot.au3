#include <ButtonConstants.au3>
#include <EditConstants.au3>
#include <GUIConstantsEx.au3>
#include <StaticConstants.au3>
#include <WindowsConstants.au3>
#include <NomadMemory.au3>

#Region ### START Koda GUI section ### Form=
$Form1 = GUICreate("BOT00 - Bot pour tester le jeu de moulin de Amine Yahemdi", 572, 120, 192, 124)
$MenuItem1 = GUICtrlCreateMenu("Configuration")
$MenuItem3 = GUICtrlCreateMenuItem("Lancer bot00.ini", $MenuItem1)
$MenuItem4 = GUICtrlCreateMenuItem("Lancer config.ini", $MenuItem1)
$MenuItem2 = GUICtrlCreateMenu("?")
$Button1 = GUICtrlCreateButton("Rapide 1v1", 16, 24, 75, 25)
$Group1 = GUICtrlCreateGroup("Mode", 8, 8, 185, 81)
GUICtrlCreateGroup("", -99, -99, 1, 1)
$Button2 = GUICtrlCreateButton("Mise à jour", 400, 48, 75, 25)
$Button3 = GUICtrlCreateButton("Lancer le bot!", 104, 56, 75, 25)
$Input1 = GUICtrlCreateInput("Processus non lancé", 384, 24, 169, 21)
GUICtrlSetState(-1, $GUI_DISABLE)
$Input2 = GUICtrlCreateInput("", 248, 24, 121, 21)
GUICtrlSetState(-1, $GUI_DISABLE)
$Label1 = GUICtrlCreateLabel("Phase de jeu actuel", 256, 0, 98, 17)
$Label2 = GUICtrlCreateLabel("Etat du jeu", 440, 0, 55, 17)
;$Button4 = GUICtrlCreateButton("Lancer le jeu", 480, 48, 75, 25)
GUISetState(@SW_SHOW)
#EndRegion ### END Koda GUI section ###

Const $processID = 18876
Global $nomad = 0
Global $game_state = -1
Const $game_state_address = 0x0040E008

Global $mode = -1

Const $MODE_RAPIDE_1V1 = 1

Global $positions[24][2]
$positions[0][0] = 423
$positions[0][1] = 149

$positions[1][0] = 622
$positions[1][1] = 146

$positions[2][0] = 822
$positions[2][1] = 148

$positions[3][0] = 823
$positions[3][1] = 349

$positions[4][0] = 823
$positions[4][1] = 550

$positions[5][0] = 622
$positions[5][1] = 550

$positions[6][0] = 422
$positions[6][1] = 550

$positions[7][0] = 422
$positions[7][1] = 349

$positions[8][0] = 475
$positions[8][1] = 200

$positions[9][0] = 620
$positions[9][1] = 200

$positions[10][0] = 775
$positions[10][1] = 200

$positions[11][0] = 775
$positions[11][1] = 350

$positions[12][0] = 775
$positions[12][1] = 500

$positions[13][0] = 620
$positions[13][1] = 500

$positions[14][0] = 475
$positions[14][1] = 500

$positions[15][0] = 475
$positions[15][1] = 350

$positions[16][0] = 525
$positions[16][1] = 250

$positions[17][0] = 625
$positions[17][1] = 250

$positions[18][0] = 725
$positions[18][1] = 250

$positions[19][0] = 725
$positions[19][1] = 350

$positions[20][0] = 725
$positions[20][1] = 450

$positions[21][0] = 625
$positions[21][1] = 450

$positions[22][0] = 525
$positions[22][1] = 450

$positions[23][0] = 525
$positions[23][1] = 350

While 1
	$nMsg = GUIGetMsg()
	Switch $nMsg
		Case $GUI_EVENT_CLOSE
			_MemoryClose($nomad)
			Exit
		 Case $Button1
			$mode = $MODE_RAPIDE_1V1
		 Case $Button3
			if $mode == $MODE_RAPIDE_1V1 then
			   local $u
				  For $i = 0 To 50
					 $u = random(0, 23, 1)

					 MouseClick("left", $positions[$u][0], $positions[$u][1], 1)

					 ;Sleep(50)
				  Next

			endif

		 Case $Button2
			Local $arr[3]
			$arr[0] = "Menu de lancement"
			$arr[1] = "Menu de jeu"
			$arr[2] = "Menu de configuration"

			$nomad = _MemoryOpen($processID)
			$game_state = _MemoryRead($game_state_address, $nomad)

			if -1 < $game_state <  4 then
			   GUICtrlSetData($Input1, $arr[$game_state])
			else
			   GUICtrlSetData($Input1, "Processus non lancé")
			endif

			_MemoryClose($nomad)
	EndSwitch
WEnd
