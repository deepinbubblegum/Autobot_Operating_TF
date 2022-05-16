#example input
adb -s emulator-5554 shell sendevent /dev/input/event4 3 53 300
adb -s emulator-5554 shell sendevent /dev/input/event4 3 54 400
adb -s emulator-5554 shell sendevent /dev/input/event4 3 48 5
adb -s emulator-5554 shell sendevent /dev/input/event4 3 58 50
adb -s emulator-5554 shell sendevent /dev/input/event4 0 2 0
adb -s emulator-5554 shell sendevent /dev/input/event4 0 0 0
adb -s emulator-5554 shell sendevent /dev/input/event4 0 2 0
adb -s emulator-5554 shell sendevent /dev/input/event4 0 0 0

adb -s emulator-5554 shell sendevent /dev/input/event4 3 53 300
adb -s emulator-5554 shell sendevent /dev/input/event4 3 53 300
adb -s emulator-5554 shell sendevent /dev/input/event4 3 48 5
adb -s emulator-5554 shell sendevent /dev/input/event4 3 58 50
adb -s emulator-5554 shell sendevent /dev/input/event4 0 2 0
adb -s emulator-5554 shell sendevent /dev/input/event4 0 0 0
adb -s emulator-5554 shell sleep 1
adb -s emulator-5554 shell sendevent /dev/input/event4 0 2 0
adb -s emulator-5554 shell sendevent /dev/input/event4 0 0 0
