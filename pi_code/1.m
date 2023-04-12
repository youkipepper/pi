a1=[]

G3=imread(picname);
G4=im2bw(G3);
ROI1=regionprops(G4(:,:));
cent1=ROI1.Centroid;
a1=[a1;cent1];