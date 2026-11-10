import fitz, sys, os
from PIL import Image
out=sys.argv[1]
jobs={"slides":"Slides/CS502/05-process-concept-pcb.pdf","notes":"Notes/CS502/05-process-concept-pcb-notes.pdf","assign":"Assignments/CS502/05-process-concept-pcb-assignment.pdf","gate":"CompetitiveExam/CS502/05-process-concept-pcb-questions.pdf","handout":"InstructorHandouts/CS502/05-process-concept-pcb-instructor-handout.pdf"}
for k,p in jobs.items():
    d=fitz.open(p); n=len(d); print(k,n)
    for i,pg in enumerate(d):
        if k!="slides" and i>3: break
        pix=pg.get_pixmap(dpi=220 if k=="slides" else 160); pix.save(f"{out}/{k}_{i+1:02d}.png")
# contact sheet of slides
ims=[Image.open(f"{out}/slides_{i:02d}.png").resize((384,216)) for i in range(1,len(fitz.open(jobs['slides']))+1)]
cols=6; rows=(len(ims)+cols-1)//cols
sheet=Image.new("RGB",(cols*384,rows*216),"white")
for j,im in enumerate(ims): sheet.paste(im,((j%cols)*384,(j//cols)*216))
sheet.save(f"{out}/../slides_sheet.png")
