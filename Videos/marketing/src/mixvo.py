import subprocess,sys
S=sys.argv[1]; voice_dir=sys.argv[2]; ext=sys.argv[3]
offs=[float(x) for x in (sys.argv[4] if len(sys.argv)>4 else '0.6,9.0,23.3,34.2,50.4,61.8,71.3,83.2,92.8,108.2,130.8,146.6,165.6,175.6').split(',')]
DUR=sys.argv[5] if len(sys.argv)>5 else '194.8'
ins=[];fl=[]
for i,o in enumerate(offs):
    ins+=['-i',f'{voice_dir}/l{i+1}.{ext}']; ms=int(o*1000)
    fl.append(f'[{i}:a]aresample=48000,pan=mono|c0=c0,adelay={ms},apad=whole_dur={DUR}[v{i}]')
fc=';'.join(fl)+';'+''.join(f'[v{i}]' for i in range(len(offs)))+'amix=inputs='+str(len(offs))+':normalize=0,pan=stereo|c0=c0|c1=c0,atrim=0:'+DUR+'[o]'
subprocess.run(['ffmpeg','-y','-loglevel','error',*ins,'-filter_complex',fc,'-map','[o]','-ar','48000',f'{S}/vo.wav'],check=True)
