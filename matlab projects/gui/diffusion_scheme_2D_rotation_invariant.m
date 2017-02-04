function u=diffusion_scheme_2D_rotation_invariant(u,Dxx,Dxy,Dyy,dt)
% Most diffusion discretizations are not rotation-invariant, probably
% because they depend on a 3x3 stencil. 
% This is an implementation of "A Scheme for Coherence-Enhancing Diffusion 
% Filtering with Optimized Rotation invariance" by Joachim Weickert. 
% Which is a basic and understandable direct discretization of the 
% diffusion equation, which corresponds to a 5x5 stencil.
% 
% Function is written by D.Kroon University of Twente (September 2009)

Dxx=double(repmat(Dxx,[1 1 size(u,3)]));
Dxy=double(repmat(Dxy,[1 1 size(u,3)]));
Dyy=double(repmat(Dyy,[1 1 size(u,3)]));

ux=double(derivatives(u,'x'));
uy=double(derivatives(u,'y'));

%% 3 : Calculate the flux components
j1 = Dxx .* ux + Dxy .*uy;
j2 = Dxy .* ux + Dyy .*uy;

%% 3.5 Flux on boundaries to zero
j1(1,:,:)=0; j1(end,:,:)=0; j1(:,1,:)=0; j1(:,end,:)=0;
j2(1,:,:)=0; j2(end,:,:)=0; j2(:,1,:)=0; j2(:,end,:)=0;

%% 4 : Calculate ... by means of the optimized derivative filters
%du = derivatives(j1,'x')+derivatives(j2,'y');
du = j1 + j2;

%% 5 : Update in an explicit way.
u=double(u)+du*dt;


	