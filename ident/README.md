# ident


# input file

```
nabeshin@iMacNabeshin ident % cat ./hoge.txt 
$Id: f.c,v 1.5 1999/08/27 23:36:42 peter Exp $
```

```
nabeshin@iMacNabeshin ident % hexdump -C libtest.out 
00000000  cf fa ed fe 07 00 00 01  03 00 00 00 02 00 00 00  |����............|
00000010  10 00 00 00 a8 05 00 00  85 00 20 00 00 00 00 00  |....�..... .....|
00000020  19 00 00 00 48 00 00 00  5f 5f 50 41 47 45 5a 45  |....H...__PAGEZE|
00000030  52 4f 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |RO..............|

...

00003f80  24 49 64 3a 20 66 2e 63  2c 76 20 31 2e 35 20 31  |$Id: f.c,v 1.5 1|
00003f90  39 39 39 2f 30 38 2f 32  37 20 32 33 3a 33 36 3a  |999/08/27 23:36:|
00003fa0  34 32 20 70 65 74 65 72  20 45 78 70 20 24 00 00  |42 peter Exp $..|
00003fb0  01 00 00 00 1c 00 00 00  00 00 00 00 1c 00 00 00  |................|
```

# Run

```
nabeshin@iMacNabeshin ident % ./a.out ./hoge.txt 
Result str[ f.c,v 1.5 1999/08/27 23:36:42 peter Exp ]
nabeshin@iMacNabeshin ident % ./a.out ./libtest.out
Result str[ f.c,v 1.5 1999/08/27 23:36:42 peter Exp ]
```