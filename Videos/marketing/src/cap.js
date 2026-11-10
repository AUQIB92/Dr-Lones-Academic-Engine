const puppeteer=require('puppeteer-core');const {spawn}=require('child_process');
const [,,html,mode,out]=process.argv; // mode: "stills:t1,t2" or "video"
(async()=>{
 const b=await puppeteer.launch({executablePath:'/Applications/Google Chrome.app/Contents/MacOS/Google Chrome',headless:'new',args:['--allow-file-access-from-files']});
 const p=await b.newPage();await p.setViewport({width:1920,height:1080,deviceScaleFactor:1});
 await p.goto('file://'+html,{waitUntil:'networkidle0'});await p.evaluate(()=>document.fonts.ready);await p.evaluate(()=>window.boot&&window.boot());await new Promise(r=>setTimeout(r,800));
 if(mode.startsWith('stills:')){for(const t of mode.slice(7).split(',')){await p.evaluate(async t=>{render(+t);if(window.ready)await ready();},t);await p.screenshot({path:`${out}/still_${t}.jpg`,type:'jpeg',quality:85});}}
 else{const fps=30;const total=await p.evaluate(()=>TOTAL);const n=Math.ceil(total*fps);
  const ff=spawn('ffmpeg',['-y','-loglevel','error','-f','image2pipe','-framerate',''+fps,'-c:v','mjpeg','-i','-','-c:v','libx264','-preset','slow','-crf','16','-pix_fmt','yuv420p',out]);
  for(let i=0;i<n;i++){await p.evaluate(async t=>{render(t);if(window.ready)await ready();},i/fps);const buf=await p.screenshot({type:'jpeg',quality:94});if(!ff.stdin.write(buf))await new Promise(r=>ff.stdin.once('drain',r));if(i%300==0)console.log('frame',i,'/',n);}
  ff.stdin.end();await new Promise(r=>ff.on('close',r));}
 await b.close();})();
