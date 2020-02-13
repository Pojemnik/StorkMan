from PIL import Image
from glob import iglob
from pathlib import Path
from progress.bar import Bar
import sys
def getSize(im):
	data=im.load()
	size=[im.size[0],im.size[1],0,0]
	for i in range(im.size[0]):
		for j in range(im.size[1]):
			if data[i,j]!=(255,255,255):
				size[0]=min(size[0],i)
				size[1]=min(size[1],j)
				size[2]=max(size[2],i)
				size[3]=max(size[3],j)
	return size


def cropAndPaste(im,toim,size,toplace):
	toim.paste(im.crop((size[0],size[1],size[2]+1,size[3]+1)),toplace)


def getSizes(files):
	minsize=[]
	with Bar("calculating sizes",max=len(files),suffix='%(percent)d%%') as bar: 
		with Image.open(files[0]) as im:
			minsize=getSize(im)
		bar.next()
		for i in range(1,len(files)):
			with Image.open(files[i]) as im:
				size=getSize(im)
				minsize[0]=min(minsize[0],size[0])
				minsize[1]=min(minsize[1],size[1])
				minsize[2]=max(minsize[2],size[2])
				minsize[3]=max(minsize[3],size[3])
				bar.next()
	return minsize

def getImSize(x):
	imsize=[1,x]
	for i in range(1,int(x**(1/2))+2):
		if x%i==0:
			if imsize[1]+imsize[0]>=i+x//i:
				imsize[0]=i
				imsize[1]=x//i
	return imsize
if len(sys.argv)!=2:
	sys.exit()
pf=sys.argv[1]
files=[i for i in iglob(pf+'\*')]
files.sort()
for i in files:
	print(Path(i).name)
minsize=getSizes(files)
imsize=getImSize(len(files))
partsize=[minsize[2]-minsize[0]+1,minsize[3]-minsize[1]+1]
outImage=Image.new("RGBA",(imsize[0]*partsize[0],imsize[1]*partsize[1]),(0,0,0,255))
with Bar("cropping and copying data",max=len(files),suffix='%(percent)d%%') as bar:
	for i in range(len(files)):
		with Image.open(files[i]) as im:
			cropAndPaste(im,outImage,minsize,(partsize[0]*(i%imsize[0]),partsize[1]*(i//imsize[0])))
			bar.next()
with Bar("adding alpha",max=imsize[0]*partsize[0],suffix='%(percent)d%%') as bar:
	dat=outImage.load()
	for i in range(outImage.size[0]):
		for j in range(outImage.size[1]):
			pix=dat[i,j]
			if pix[2]==pix[1]==pix[0]==255:
				dat[i,j]=(pix[0],pix[1],pix[2],0)
		bar.next()
print("single size",partsize)
print("image size",imsize)
outImage.save(pf+"_ss_{}_{}_is_{}_{}".format(partsize[0],partsize[1],imsize[0],imsize[1])+Path(files[0]).suffix)
