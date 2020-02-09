# 68KatyClone
An open-hardware design for an 8-bit Motorola 68008 system.
Please feel free to re-use any of the parts enclosed in this repository, or see the [license](./LICENSE) if in doubt.

This repository is my attempt to build a re-vamped edition of Steve "Big Mess-O-Wires" Chamberlin's [68Katy](https://www.bigmessowires.com/68-katy/), for which I owe him many thanks. It's my first go at breadboard prototyping, but since I'm learning so much I thought I'd contribute by publishing my findings. It has been quite the ride thus far. Also, I'd like to point to Ben Eater's [6502 project](https://eater.net/6502), which has been quite inspirational. 

I'm using the [68Katy](https://www.bigmessowires.com/68-katy/) design because it's pretty well documented, as Motorola 68K projects go. There's a lot of people experimenting, I even believe there's some kind of revival going of people baking 8-bit systems just for fun. The fact that a lot of the components can be ordered over the mail very cheaply most certainly adds to the fun - parts are available in abundance and even the shipping is often very cheap if you're willing to wait a couple of weeks. My guess is that these kinds of projects are being undertaken by late-30's to mid-50's guys tinkering away in sheds and attics. I happen to be just one of them. 

The reason I wanted to do a 68K project, is because I used to be, and still am, the proud owner of a Texas Instruments TI-92 that I bought in high school. At the time, I thought of it as a marvel of technology. I dabbled unsuccessfully in some C++ coding (I found it too hard), but the TI-92 allowed on-board BASIC programming an fancy graph plotting. So there my journey began. Now, over 20 years later, I'm a professional software developer and I believe I'm now better equipped for taking on the challenge of building an 8-bit system from bare components.

# Schematics
The [KiCad](KiCad) folder contains a work-in-progress translation of the schematics from Steve Chamberlin. I'm working on a board but at the moment it's not working. So use at your own peril.

# Software
## ROM image
The [ROM](Software/ROM) folder contains the ROM image: the monitor program and the Linux image. Send it to the ROM using a ROM programmer, I used a [TL866II plus](https://www.aliexpress.com/item/32963724045.html)

## GAL image
The [GAL/68katy](Software/GAL/68katy) folder contains the .jed compiled image. Send it to the GAL using a GAL-compatible ROM programmer, I used a [TL866II plus](https://www.aliexpress.com/item/32963724045.html)
For good measure I also included the [OpalJR compiler](Software/GAL/opaljr21) - as the license seems to allow it; I got it from [here](http://www.brouhaha.com/~eric/retrocomputing/mmi/palasm/opaljr21.zip). There's no reason not to include it here - it's the same program the original 68Katy GAL used to compile so it *should* be OK, although I can never be 100% sure unless I compare the JEDEC compiled file to the original (which I can't). Since it's a 16-bit DOS program, you need something like [DOSBox](https://www.dosbox.com/download.php?main=1) to run it.