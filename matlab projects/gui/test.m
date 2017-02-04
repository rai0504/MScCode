function varargout = test(varargin)
% TEST MATLAB code for test.fig
%      TEST, by itself, creates a new TEST or raises the existing
%      singleton*.
%
%      H = TEST returns the handle to a new TEST or the handle to
%      the existing singleton*.
%
%      TEST('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TEST.M with the given input arguments.
%
%      TEST('Property','Value',...) creates a new TEST or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before test_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to test_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help test

% Last Modified by GUIDE v2.5 25-Jan-2017 22:27:58

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @test_OpeningFcn, ...
                   'gui_OutputFcn',  @test_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before test is made visible.
function test_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to test (see VARARGIN)

% Choose default command line output for test
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes test wait for user response (see UIRESUME)
% uiwait(handles.figure1);

f = double(imread('test_set/188gray.jpg'));
f = f(:,:,1);
N = size(f); N = N(1);

%--------------------------------------------------------------
% TV Anisotropic Denoising
mu = 5;
f_denoise_atv = SB_ATV(f,mu);
fprintf('ATV Rel.Err = %g\n',norm(f_denoise_atv(:) - f(:)) / norm(f(:)));
f = uint8(reshape(f_denoise_atv,N,N));
%figure; 
%colormap gray; 
%imagesc(f);
imwrite(f,'188grayTv.jpg');

%--------------------------------------------------------------
% CEDORI
Options=struct('sigma', 0.5, 'rho', 1, 'TensorType', 1, 'eigenmode',3,'C', 1e-10, 'm',1,'alpha',0.001,'lambda_e',0.02,'lambda_c',0.02,'lambda_h',0.5,'RealDerivatives',false);
dt = 0.01;
n = 50;
u=double(f)/255;
for i=1:n
% Gaussian smooth the image, for better gradients
u=imgaussian(u,Options.sigma,Options.sigma);

% Calculate the gradients
ux=derivatives(u,'x'); 
uy=derivatives(u,'y');

% Compute the 2D structure tensors J of the image
[Jxx, Jxy, Jyy] = StructureTensor2D(ux,uy,Options.rho);

% Compute the eigenvectors and values of the strucure tensors, v1 and v2, mu1 and mu2
[mu1,mu2,v1x,v1y,v2x,v2y]=EigenVectors2D(Jxx,Jxy,Jyy);

% Gradient magnitude squared
gradA=ux.^2+uy.^2;

% Construct the edge preserving diffusion tensors D = [Dxx,Dxy;Dxy,Dyy]
[Dxx,Dxy,Dyy]=ConstructDiffusionTensor2D(mu1,mu2,v1x,v1y,v2x,v2y,gradA,Options);

kappa = (Jxx-Jyy).^2 + 4*(Jxy.*Jxy);
lambda_1 = Options.alpha;
lambda_2 = lambda_1 +(1-lambda_1)*exp(-(Options.C)./kappa);

u = diffusion_scheme_2D_rotation_invariant(u,Dxx,Dxy,Dyy,dt);
imshow(u);
end
for r=1:256
    for c=1:256
        if u(r,c)>1
            u(r,c)=1;
        end
    end
end
%g = u/255;
imshow(u);
u = uint8(u*255);
imwrite(u,'188grayTvCed.jpg');

% --- Outputs from this function are returned to the command line.
function varargout = test_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;
