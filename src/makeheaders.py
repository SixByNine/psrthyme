#!/usr/bin/python

from sys import argv,exit

infiles=argv[0:]




for cppfile in infiles:
    with open(cppfile,"r") as inf:
        if cppfile.endswith(".cpp"):
            hppfile=cppfile.replace(".cpp",".hpp")
            macro_id=hppfile.upper().replace(".","_").replace("+","_").replace("-","_")
            outf=open(hppfile,'w')
            outf.write("// Auto generated .hpp file\n")
            outf.write("#ifndef _%s\n"%macro_id)
            outf.write("#define _%s\n"%macro_id)
            ifdepth=0
            INTERFACE=0
            for line in inf:
                line=line.strip()
                if line.startswith("#if"):
                    ifdepth+=1
                if line.startswith("#endif"):
                    ifdepth-=1
                if line.startswith("#if INTERFACE"):
                    INTERFACE=ifdepth
                    outf.write("#undef INTERFACE\n")
                    continue
                if ifdepth<INTERFACE:
                    INTERFACE=0

                if INTERFACE > 0:
                    outf.write(line+"\n")


            outf.write("#endif // ifndef _%s\n"%macro_id)
