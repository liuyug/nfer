# Network Forwarder
Transfer socket stream to another.

# Compile


``` sh
mkdir build
cd build

# for mingw
# cmake -G "MinGW Makefiles" ../
# for make or nmake
cmake ../

make clean
make
```

+ Help
```
make help
make VERBOSE=1

```

# Run

``` sh
Network Forwarder v1.0
Copyright (C) LIU Yugang <liuyug@gmail.com>

Usage: nfer [options]...
   -h, --help          print this message
   -v <number>         verbose level, 0-9.

options:
   -c <host:port>      connect to host:port
   -l <host:port>      listen on host:port
   --http-host         fix HTTP host

```

## Example

access  ```www.microsoft.com``` site.


``` sh
$ ./nfer -v 1 -c www.microsoft.com:80 -l 127.0.0.1:9999 --http-host

Network Forwarder v1.0
Copyright (C) LIU Yugang <liuyug@gmail.com>

listen on 127.0.0.1:9999
connect to www.microsoft.com:80
connected from 127.0.0.1:5031 to 127.0.0.1:9999
recv 78 bytes from 127.0.0.1:5031
fix http host, origin:
00000000 47 45 54 20 2F 20 48 54  54 50 2F 31 2E 31 0D 0A  |GET / HTTP/1.1..|
00000010 48 6F 73 74 3A 20 31 32  37 2E 30 2E 30 2E 31 3A  |Host: 127.0.0.1:|
00000020 39 39 39 39 0D 0A 55 73  65 72 2D 41 67 65 6E 74  |9999..User-Agent|
00000030 3A 20 63 75 72 6C 2F 37  2E 34 37 2E 30 0D 0A 41  |: curl/7.47.0..A|
00000040 63 63 65 70 74 3A 20 2A  2F 2A 0D 0A 0D 0A        |ccept: */*....|
send 81 bytes to www.microsoft.com:80
00000000 47 45 54 20 2F 20 48 54  54 50 2F 31 2E 31 0D 0A  |GET / HTTP/1.1..|
00000010 48 6F 73 74 3A 20 77 77  77 2E 6D 69 63 72 6F 73  |Host: www.micros|
00000020 6F 66 74 2E 63 6F 6D 0D  0A 55 73 65 72 2D 41 67  |oft.com..User-Ag|
00000030 65 6E 74 3A 20 63 75 72  6C 2F 37 2E 34 37 2E 30  |ent: curl/7.47.0|
00000040 0D 0A 41 63 63 65 70 74  3A 20 2A 2F 2A 0D 0A 0D  |..Accept: */*...|
00000050 0A                                                |.|
recv 1284 bytes from www.microsoft.com:80
send 1284 bytes to 127.0.0.1:5031
00000000 48 54 54 50 2F 31 2E 31  20 32 30 30 20 4F 4B 0D  |HTTP/1.1 200 OK.|
00000010 0A 53 65 72 76 65 72 3A  20 41 70 61 63 68 65 0D  |.Server: Apache.|
00000020 0A 45 54 61 67 3A 20 22  36 30 38 32 31 35 31 62  |.ETag: "6082151b|
00000030 64 35 36 65 61 39 32 32  65 31 33 35 37 66 35 38  |d56ea922e1357f58|
00000040 39 36 61 39 30 64 30 61  3A 31 34 32 35 34 35 34  |96a90d0a:1425454|
00000050 37 39 34 22 0D 0A 4C 61  73 74 2D 4D 6F 64 69 66  |794"..Last-Modif|
00000060 69 65 64 3A 20 57 65 64  2C 20 30 34 20 4D 61 72  |ied: Wed, 04 Mar|
00000070 20 32 30 31 35 20 30 37  3A 33 39 3A 35 34 20 47  | 2015 07:39:54 G|
00000080 4D 54 0D 0A 41 63 63 65  70 74 2D 52 61 6E 67 65  |MT..Accept-Range|
00000090 73 3A 20 62 79 74 65 73  0D 0A 43 6F 6E 74 65 6E  |s: bytes..Conten|
000000A0 74 2D 4C 65 6E 67 74 68  3A 20 31 30 32 30 0D 0A  |t-Length: 1020..|
000000B0 43 6F 6E 74 65 6E 74 2D  54 79 70 65 3A 20 74 65  |Content-Type: te|
000000C0 78 74 2F 68 74 6D 6C 0D  0A 44 61 74 65 3A 20 53  |xt/html..Date: S|
000000D0 75 6E 2C 20 30 35 20 4E  6F 76 20 32 30 31 37 20  |un, 05 Nov 2017 |
000000E0 30 35 3A 32 30 3A 34 31  20 47 4D 54 0D 0A 43 6F  |05:20:41 GMT..Co|
000000F0 6E 6E 65 63 74 69 6F 6E  3A 20 6B 65 65 70 2D 61  |nnection: keep-a|
00000100 6C 69 76 65 0D 0A 0D 0A  3C 68 74 6D 6C 3E 3C 68  |live....<html><h|
00000110 65 61 64 3E 3C 74 69 74  6C 65 3E 4D 69 63 72 6F  |ead><title>Micro|
00000120 73 6F 66 74 20 43 6F 72  70 6F 72 61 74 69 6F 6E  |soft Corporation|
00000130 3C 2F 74 69 74 6C 65 3E  3C 6D 65 74 61 20 68 74  |</title><meta ht|
00000140 74 70 2D 65 71 75 69 76  3D 22 58 2D 55 41 2D 43  |tp-equiv="X-UA-C|
00000150 6F 6D 70 61 74 69 62 6C  65 22 20 63 6F 6E 74 65  |ompatible" conte|
00000160 6E 74 3D 22 49 45 3D 45  6D 75 6C 61 74 65 49 45  |nt="IE=EmulateIE|
00000170 37 22 3E 3C 2F 6D 65 74  61 3E 3C 6D 65 74 61 20  |7"></meta><meta |
00000180 68 74 74 70 2D 65 71 75  69 76 3D 22 43 6F 6E 74  |http-equiv="Cont|
00000190 65 6E 74 2D 54 79 70 65  22 20 63 6F 6E 74 65 6E  |ent-Type" conten|
000001A0 74 3D 22 74 65 78 74 2F  68 74 6D 6C 3B 20 63 68  |t="text/html; ch|
000001B0 61 72 73 65 74 3D 75 74  66 2D 38 22 3E 3C 2F 6D  |arset=utf-8"></m|
000001C0 65 74 61 3E 3C 6D 65 74  61 20 6E 61 6D 65 3D 22  |eta><meta name="|
000001D0 53 65 61 72 63 68 54 69  74 6C 65 22 20 63 6F 6E  |SearchTitle" con|
000001E0 74 65 6E 74 3D 22 4D 69  63 72 6F 73 6F 66 74 2E  |tent="Microsoft.|
000001F0 63 6F 6D 22 20 73 63 68  65 6D 65 3D 22 22 3E 3C  |com" scheme=""><|
00000200 2F 6D 65 74 61 3E 3C 6D  65 74 61 20 6E 61 6D 65  |/meta><meta name|
00000210 3D 22 44 65 73 63 72 69  70 74 69 6F 6E 22 20 63  |="Description" c|
00000220 6F 6E 74 65 6E 74 3D 22  47 65 74 20 70 72 6F 64  |ontent="Get prod|
00000230 75 63 74 20 69 6E 66 6F  72 6D 61 74 69 6F 6E 2C  |uct information,|
00000240 20 73 75 70 70 6F 72 74  2C 20 61 6E 64 20 6E 65  | support, and ne|
00000250 77 73 20 66 72 6F 6D 20  4D 69 63 72 6F 73 6F 66  |ws from Microsof|
00000260 74 2E 22 20 73 63 68 65  6D 65 3D 22 22 3E 3C 2F  |t." scheme=""></|
00000270 6D 65 74 61 3E 3C 6D 65  74 61 20 6E 61 6D 65 3D  |meta><meta name=|
00000280 22 54 69 74 6C 65 22 20  63 6F 6E 74 65 6E 74 3D  |"Title" content=|
00000290 22 4D 69 63 72 6F 73 6F  66 74 2E 63 6F 6D 20 48  |"Microsoft.com H|
000002A0 6F 6D 65 20 50 61 67 65  22 20 73 63 68 65 6D 65  |ome Page" scheme|
000002B0 3D 22 22 3E 3C 2F 6D 65  74 61 3E 3C 6D 65 74 61  |=""></meta><meta|
000002C0 20 6E 61 6D 65 3D 22 4B  65 79 77 6F 72 64 73 22  | name="Keywords"|
000002D0 20 63 6F 6E 74 65 6E 74  3D 22 4D 69 63 72 6F 73  | content="Micros|
000002E0 6F 66 74 2C 20 70 72 6F  64 75 63 74 2C 20 73 75  |oft, product, su|
000002F0 70 70 6F 72 74 2C 20 68  65 6C 70 2C 20 74 72 61  |pport, help, tra|
00000300 69 6E 69 6E 67 2C 20 4F  66 66 69 63 65 2C 20 57  |ining, Office, W|
00000310 69 6E 64 6F 77 73 2C 20  73 6F 66 74 77 61 72 65  |indows, software|
00000320 2C 20 64 6F 77 6E 6C 6F  61 64 2C 20 74 72 69 61  |, download, tria|
00000330 6C 2C 20 70 72 65 76 69  65 77 2C 20 64 65 6D 6F  |l, preview, demo|
00000340 2C 20 20 62 75 73 69 6E  65 73 73 2C 20 73 65 63  |,  business, sec|
00000350 75 72 69 74 79 2C 20 75  70 64 61 74 65 2C 20 66  |urity, update, f|
00000360 72 65 65 2C 20 63 6F 6D  70 75 74 65 72 2C 20 50  |ree, computer, P|
00000370 43 2C 20 73 65 72 76 65  72 2C 20 73 65 61 72 63  |C, server, searc|
00000380 68 2C 20 64 6F 77 6E 6C  6F 61 64 2C 20 69 6E 73  |h, download, ins|
00000390 74 61 6C 6C 2C 20 6E 65  77 73 22 20 73 63 68 65  |tall, news" sche|
000003A0 6D 65 3D 22 22 3E 3C 2F  6D 65 74 61 3E 3C 6D 65  |me=""></meta><me|
000003B0 74 61 20 6E 61 6D 65 3D  22 53 65 61 72 63 68 44  |ta name="SearchD|
000003C0 65 73 63 72 69 70 74 69  6F 6E 22 20 63 6F 6E 74  |escription" cont|
000003D0 65 6E 74 3D 22 4D 69 63  72 6F 73 6F 66 74 2E 63  |ent="Microsoft.c|
000003E0 6F 6D 20 48 6F 6D 65 70  61 67 65 22 20 73 63 68  |om Homepage" sch|
000003F0 65 6D 65 3D 22 22 3E 3C  2F 6D 65 74 61 3E 3C 2F  |eme=""></meta></|
00000400 68 65 61 64 3E 3C 62 6F  64 79 3E 3C 70 3E 59 6F  |head><body><p>Yo|
00000410 75 72 20 63 75 72 72 65  6E 74 20 55 73 65 72 2D  |ur current User-|
00000420 41 67 65 6E 74 20 73 74  72 69 6E 67 20 61 70 70  |Agent string app|
00000430 65 61 72 73 20 74 6F 20  62 65 20 66 72 6F 6D 20  |ears to be from |
00000440 61 6E 20 61 75 74 6F 6D  61 74 65 64 20 70 72 6F  |an automated pro|
00000450 63 65 73 73 2C 20 69 66  20 74 68 69 73 20 69 73  |cess, if this is|
00000460 20 69 6E 63 6F 72 72 65  63 74 2C 20 70 6C 65 61  | incorrect, plea|
00000470 73 65 20 63 6C 69 63 6B  20 74 68 69 73 20 6C 69  |se click this li|
00000480 6E 6B 3A 3C 61 20 68 72  65 66 3D 22 68 74 74 70  |nk:<a href="http|
00000490 3A 2F 2F 77 77 77 2E 6D  69 63 72 6F 73 6F 66 74  |://www.microsoft|
000004A0 2E 63 6F 6D 2F 65 6E 2F  75 73 2F 64 65 66 61 75  |.com/en/us/defau|
000004B0 6C 74 2E 61 73 70 78 3F  72 65 64 69 72 3D 74 72  |lt.aspx?redir=tr|
000004C0 75 65 22 3E 55 6E 69 74  65 64 20 53 74 61 74 
```


