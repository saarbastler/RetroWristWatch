# RetroWristWatch
Retro wristwatch using a 4 digit bubble display

![Retro Wrist Watch](/images/title.png)

## Manual:
Left Button: Show Date (DD.MM), left Button again: show year
Right Button: Show Time (HH MM), right Button again: show seconds

Setting Time:
First push the right Button, time is shown, now push right Button longer ~2s
Left digit is flashing, right button will count up, left button will select the next digit.
Push left Button long to save the time, push right button long to cancel

Setting Date:
First push the left Button, date is shown, now push right Button longer ~2s.
Day is flashing, right button will count up, left button will select month.
Push left Button long to save the time, push right button long to cancel

Setting Year:
First push the left Button, date is shown, now push left Button again, Year is shown. Push the right button longer ~2s.
Lower two digits are flashing, right button will count up, left button will select upper two digits.
Push left Button long to save the time, push right button long to cancel

## The PCB
The PCB is made using [Target](https://ibfriedrich.com/en/index.html), a German PCB software. Detail see in the [pcb](pcb) folder.

## The software
The Software is written using Atmel Studio 7 in C++. I'm not using any library, its coded "bare metal". Compiling as Debug will use the UART to print debug messages and allow a few commands. In Release mode the UART is not used.
Some source files are an early version of my [AvrLibrary](https://github.com/saarbastler/AvrLibrary).
 