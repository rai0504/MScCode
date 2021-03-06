clc
clear
close all

image_set = ['188  ';'195  ';'228  ';'1057 ';'1265 ';'10093';'10102';'10104';'12294';'12627';'13432';'13438';'13899';...
    '13901';'21749';'21759';'32140';'35278';'37338';'37339';'38974';'40217';'40968';'41066';'42451'];
%image_set = ['1';'2';'3';'4';'5';'6'];
image_set = cellstr(image_set);

imageNumber = 25;
%groundtruth = rgb2gray(imread(strcat('\test_set_ground_truth\',char(image_set(imageNumber)),'gt.png')));
groundtruth = imread(strcat('\test_set_ground_truth_1\',char(image_set(imageNumber)),'gt.png'));
%figure, imshow(groundtruth);

segmentationmask = imread(strcat('\origseg\def',char(image_set(imageNumber)),'.jpg'));
%figure, imshow(segmentationmask);
redChannel = segmentationmask(:,:,1);
%figure, imshow(redChannel);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% Classify each pixel
tp=0;   % ground truth = 1, segmentation mask = 1
tn=0;   % ground truth = 0, segmentation mask = 0
fp=0;   % ground truth = 0, segmentation mask = 1
fn=0;   % ground truth = 1, segmentation mask = 0

p=0;
n=0;

for r=1:256
    for c=1:256
        gt = groundtruth(r,c)>=127;
        if (gt)
            p = p+1;
        else
            n = n+1;
        end
        mask = redChannel(r,c)>=127;
        if (gt==1 && mask==1) 
            tp = tp+1;
        elseif (gt==0 && mask==0)
            tn = tn+1;
        elseif (gt==0 && mask==1)
            fp = fp+1;
        elseif (gt==1 && mask==0)
            fn = fn+1;
        end
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% Binary Classification Efficiency Measures
precision = tp/(tp+fp);
recall = tp/(tp+fn);
accuracy = (tp+tn)/(p+n);
mcc = (tp*tn-fp*fn)/(sqrt((tp+fp)*(tp+fn)*(tn+fp)*(tn+fn)));

fprintf('Image %d\nTP = %d\nTN = %d\nFP = %d\nFN = %d\n',imageNumber,tp,tn,fp,fn);
fprintf('Precision = %f\nRecall = %f\nAccuracy = %f\nMCC = %f\n',precision,recall,accuracy,mcc);
%fprintf('Accuracy = %f\n',accuracy);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%% Plots: Precision vs Recall, Accuracy, MCC
preDefault = [0.999658,0.843105,0.708847,0.781957,0.763581,0.988897,0.950973,0.998653,0.816646,...
    0.996465,0.800120,0.747243,0.452775,0.625288,0.518621,0.607195,0.964840,0.997321,0.857840,0.850256,...
    0.968461,0.997928,0.948362,0.359433,0.989382];
recDefault = [0.950275,0.971111,1.000000,1.000000,1.000000,0.935899,0.822381,0.888838,0.999838,...
    0.999971,0.972695,0.993308,0.986210,0.934609,1.000000,1.000000,0.944797,0.841366,0.941426,0.962720,...
    0.992700,0.916084,0.998812,0.999746,0.853762];
accDefault = [0.969437,0.929810,0.926834,0.933655,0.883453,0.954712,0.938950,0.925629,0.957748,...
    0.998123,0.898193,0.897644,0.805222,0.869812,0.952271,0.926529,0.985779,0.908768,0.946838,0.958603,...
    0.993042,0.961441,0.986435,0.786255,0.920547];
mccDefault = [];

preCed = [0.999841,0.826888,0.822682,0.840774,0.814105,0.992207,0.944749,0.998891,0.898883,...
    0.994417,0.799654,0.767513,0.451761,0.621311,0.514661,0.891732,0.666752,0.988902,0.864786,0.851018,...
    0.982593,0.987728,0.983554,0.617282,0.980069];
recCed = [0.941354,0.969425,1.000000,1.000000,1.000000,0.947312,0.918669,0.913804,0.998864,0.999651,...
    0.974720,0.990140,0.986304,0.935269,1.000000,0.992611,1.000000,0.848820,0.889074,0.962415,0.932041,...
    0.912959,0.990686,0.993258,0.868774];
accCed = [0.964096,0.921799,0.966766,0.954941,0.914047,0.963501,0.962524,0.942276,0.978653,0.996872,...
    0.898499,0.907471,0.804428,0.867874,0.951508,0.985474,0.920700,0.908875,0.938080,0.958755,0.985168,...
    0.955795,0.993683,0.925323,0.923874];
mccCed = [];

preTv = [0.999867,0.979666,0.866289,0.811427,0.986133,0.878567,0.999872,0.882425,0.990234,0.767056,...
    0.736864,0.390124,0.566975,0.323657,0.903570,0.574063,0.993297,0.861993,0.812980,0.936216,0.969411,...
    0.978166,0.474865,0.946257];
recTv = [0.937531,0.939238,0.999679,1.000000,0.959981,0.973550,0.897344,0.997972,0.999535,0.979298,...
    0.979361,0.905167,0.785353,0.975964,0.989520,1.000000,0.865343,0.987654,0.992224,0.878856,0.906201,...
    0.994186,0.998601,0.883845];
accTv = [0.961777,0.888000,0.987625,0.963211,0.912521,0.967514,0.955322,0.931976,0.974609,0.994583,...
    0.880188,0.889374,0.756088,0.830475,0.893890,0.986816,0.882278,0.920334,0.959549,0.952759,0.968216,...
    0.945007,0.993164,0.867371,0.914948];
mccTv = [];

preProp = [0.992634,0.863342,0.952513,0.859639,0.829628,0.994904,0.906218,0.999140,0.931884,0.990805,...
    0.857056,0.941733,0.300891,0.525781,0.994418,0.900461,0.999603,0.986931,0.996922,0.876266,0.855523,...
    0.941207,0.967293,0.042939,0.990897];
recProp = [0.979885,0.924633,1.000000,1.000000,1.000000,0.957040,0.925653,0.937295,0.994483,0.999593,...
    0.918409,0.813282,0.360631,0.977788,0.687240,0.971114,0.967878,0.962507,0.997475,0.982618,0.858869,...
    0.917170,0.990936,0.063860,0.890113];
accProp = [0.983276,0.925751,0.991119,0.961151,0.922699,0.970947,0.952789,0.957962,0.985291,0.994919,...
    0.911575,0.929199,0.761353,0.811813,0.983719,0.984528,0.994843,0.977325,0.998611,0.968750,0.949753,...
    0.937042,0.989609,0.716980,0.939636];
mccProp = [];
% 
% preEMGMM = [0.999841,0.758310,0.897504,0.926800,0.819078,0.999391,0.975491,1.000000,0.979073,0.999041,...
%     0.738885,0.744616,0.556659,0.683813,0.930977,0.887960,0.652206,0.986227,1.000000,0.998607,...
%     0.998934,0.994306,0.948080,0.426162,0.990867];
% recEMGMM = [0.942929,0.996901,1.000000,0.951645,1.000000,0.866218,0.906131,0.406681,0.994483,0.999273,...
%     0.986347,0.994381,0.899688,0.757203,0.992582,0.998791,0.933606,0.951885,0.150473,0.874209,...
%     0.814026,0.966685,0.998812,0.999618,0.699406];
% accEMGMM = [0.965057,0.892563,0.982391,0.970612,0.916855,0.918472,0.967636,0.607285,0.994965,...
%     0.999115,0.863571,0.896454,0.868042,0.876587,0.995834,0.985550,0.917786,0.965179,0.798416,0.974579,...
%     0.967194,0.982544,0.986359,0.838501,0.843018];
% mccEMGMM = [];
% 
% preDilate = [0.999920,0.710244,0.864488,0.917025,0.819323,0.993380,0.966874,0.999302,0.955184,...
%     0.996089,0.809009,0.763433,0.496870,0.715877,0.840540,0.840339,0.641773,0.992678,0.925786,0.863208,...
%     0.998617,0.969611,0.948137,0.505575,0.972836];
% recDilate = [0.933633,0.996400,1.000000,0.956840,1.000000,0.950579,0.810558,0.561298,0.997728,0.999884,...
%     0.973505,0.991469,0.974780,0.744374,0.997923,0.999194,0.943135,0.937677,0.973828,0.990547,0.815677,...
%     0.990389,0.998812,0.997837,0.948071];
% accDilate = [0.959427,0.862671,0.975830,0.969131,0.916992,0.966156,0.939728,0.709366,0.990768,0.997879,...
%     0.903458,0.905685,0.836472,0.885300,0.990158,0.978348,0.915009,0.960739,0.975266,0.966690,0.967438,...
%     0.981735,0.986374,0.882690,0.959915];
% mccDilate = [];
% 
% 
imageset = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25];
figure
plot(preDefault,recDefault,'xk',...
    preCed,recCed,'xc',...
    preTv,recTv,'xr',...
    preProp,recProp,'xb');
xlabel('Precision');
ylabel('Recall');
rect = [0.21 0.175 0.2 0.1]; % position of legend
l=legend('None','CED','TV','Proposed');
set(l,'Position',rect);

figure
plot(imageset,accDefault,'- .k',...
    imageset,accCed,'- .c',...
    imageset,accTv,'- .r',...
    imageset,accProp,'- .b');
xlabel('Image');
ylabel('Accuracy');
rect = [0.21 0.175 0.2 0.1]; % position of legend
l=legend('None','CED','TV','Proposed');
set(l,'Position',rect);

fprintf('\n\nStats\t\t\t\tMean\t\tStd. Dev\n');
fprintf('Default(Precision)\t%f\t%f\n',   mean(preDefault),std(preDefault));
fprintf('Default(Recall)\t\t%f\t%f\n',    mean(recDefault),std(recDefault));
fprintf('Default(Accuracy)\t%f\t%f\n',  mean(accDefault),std(accDefault));
fprintf('Default(MCC)\t\t%f\t%f\n', mean(mccDefault),std(mccDefault));

fprintf('CED(Precision)\t%f\t%f\n',   mean(preCed),std(preCed));
fprintf('CED(Recall)\t\t%f\t%f\n',    mean(recCed),std(recCed));
fprintf('CED(Accuracy)\t\t%f\t%f\n',  mean(accCed),std(accCed));
fprintf('CED(MCC)\t\t\t%f\t\t\t%f\n', mean(mccCed),std(mccCed));

fprintf('TV(Precision)\t\t%f\t%f\n', mean(preTv),std(preTv));
fprintf('TV(Recall)\t\t%f\t%f\n',    mean(recTv),std(recTv));
fprintf('TV(Accuracy)\t\t%f\t%f\n',  mean(accTv),std(accTv));
fprintf('TV(MCC)\t\t\t%f\t\t\t%f\n', mean(mccTv),std(mccTv));

fprintf('Proposed(Precision)\t%f\t%f\n',  mean(preProp),std(preProp));
fprintf('Proposed(Recall)\t\t%f\t%f\n',   mean(recProp),std(recProp));
fprintf('Proposed(Accuracy)\t%f\t%f\n',   mean(accProp),std(accProp));
fprintf('Proposed(MCC)\t\t\t%f\t\t\t%f\n',mean(mccProp),std(mccProp));
% 
% fprintf('EMGMM(Precision)\t%f\t%f\n',  mean(preEMGMM),std(preEMGMM));
% fprintf('EMGMM(Recall)\t\t%f\t%f\n',   mean(recEMGMM),std(recEMGMM));
% fprintf('EMGMM(Accuracy)\t\t%f\t%f\n', mean(accEMGMM),std(accEMGMM));
% fprintf('EMGMM(MCC)\t\t\t%f\t\t\t%f\n',mean(mccEMGMM),std(mccEMGMM));
% 
% fprintf('Dilate(Precision)\t%f\t%f\n',  mean(preDilate),std(preDilate));
% fprintf('Dilate(Recall)\t\t%f\t%f\n',   mean(recDilate),std(recDilate));
% fprintf('Dilate(Accuracy)\t%f\t%f\n',   mean(accDilate),std(accDilate));
% fprintf('Dilate(MCC)\t\t\t%f\t\t\t%f\n',mean(mccDilate),std(mccDilate));

%seg = [0.995056,0.992844,0.984787,0.962433,0.981140,0.985962];
%fprintf('%f\n%f\n',  mean(seg),std(seg));
% % 
% % preE = [0.978445,0.883922,0.961329,0.647059,0.746642,0.998434,0.697598,0.995262,0.979787,0.595577,...
% %     0.914233,0.785550,0.316264,0.593505,0.280906,0.707869,0.663843,0.969193,0.606324,0.864183,0.348376,...
% %     0.775897,0.908900,0.640918,0.818427];
% % preB = [0.947084,0.879816,0.964218,1.000000,0.952089,0.998712,0.652013,0.986118,0.959246,0.587096,...
% %     0.902778,0.784131,0.321245,0.558277,0.228878,0.683346,0.629321,0.953659,0.564875,0.952004,0.191016,...
% %     0.768528,0.877702,0.599372,0.985606];
% % preR = [0.999292,0.824963,0.977507,0.852403,0.892541,0.997631,0.792239,0.993598,0.985726,0.998748,...
% %     0.951336,0.783893,0.494478,0.561635,0.995510,0.919021,0.614869,0.987469,0.719509,0.906641,0.975693,...
% %     0.797907,0.984563,0.726407,0.899835];
% % preS = [0.999292,0.824963,0.977507,0.852542,0.892673,0.997631,0.792239,0.993598,0.985726,0.998748,...
% %     0.951336,0.783893,0.494478,0.561635,0.995510,0.919021,0.614869,0.987469,0.719509,0.906641,0.975693,...
% %     0.797907,0.984563,0.726407,0.899932];
% % 
% % recE = [0.997026,0.951426,0.993864,0.046559,0.081114,0.913476,0.996701,0.992784,0.983125,0.999186,...
% %     0.931048,0.975326,0.082837,0.975295,0.985757,0.999463,0.894439,0.838971,1.000000,0.925059,0.996263,...
% %     0.996129,0.999750,0.991095,0.144746];
% % recB = [1.000000,0.930329,1.000000,0.038864,0.069277,0.916040,0.999945,0.997326,0.994889,1.000000,...
% %     0.940083,0.979413,0.117975,0.990910,1.000000,1.000000,0.901770,0.915960,1.000000,0.952657,0.999218,...
% %     0.997657,1.000000,0.995166,0.038828];
% % recR = [0.952649,0.993256,0.928946,1.000000,0.977421,0.899801,0.982403,0.994698,0.980448,0.997354,...
% %     0.852171,0.983040,0.976953,0.990983,0.723739,0.983474,0.961357,0.962730,0.999614,0.972097,0.973234,...
% %     0.997045,0.984810,0.980155,0.990927];
% % recS = [0.952649,0.993256,0.928946,1.000000,0.977421,0.899801,0.982403,0.994698,0.980448,0.997354,...
% %     0.852171,0.983040,0.976953,0.990983,0.723739,0.983474,0.961357,0.962730,0.999614,0.972097,0.973234,...
% %     0.997045,0.984810,0.980155,0.990927];
% % 
% % accE = [0.984772,0.941895,0.992889,0.767105,0.643753,0.946609,0.879196,0.992096,0.993011,0.643555,...
% %     0.941132,0.913101,0.822906,0.855820,0.869507,0.953094,0.918625,0.893570,0.845932,0.955902,0.672150,...
% %     0.868988,0.975479,0.932327,0.546310];
% % accB = [0.965881,0.934113,0.994278,0.771317,0.648346,0.948318,0.851898,0.988937,0.991089,0.630966,...
% %     0.939301,0.913315,0.817245,0.834915,0.826752,0.947372,0.908295,0.927109,0.817215,0.980911,0.256821,...
% %     0.863937,0.965988,0.919632,0.508286];
% % accR = [0.970673,0.927170,0.985748,0.958801,0.947205,0.937881,0.923630,0.992249,0.993652,0.997955,...
% %     0.927902,0.913986,0.834930,0.837128,0.985626,0.988281,0.906631,0.971939,0.907440,0.974380,0.991043,...
% %     0.885208,0.992523,0.953339,0.938965];
% % accS = [0.970673,0.927170,0.985748,0.958847,0.947266,0.937881,0.923630,0.992249,0.993652,0.997955,...
% %     0.927902,0.913986,0.834930,0.837128,0.985626,0.988281,0.906631,0.971939,0.907440,0.974380,0.991043,...
% %     0.885208,0.992523,0.953339,0.939026];
% % 
% % mccE = [0.968092,0.873376,0.973300,0.126807,0.157788,0.895655,0.760449,0.982368,0.977150,0.384800,...
% %     0.875182,0.816694,0.088365,0.684384,0.487566,0.818543,0.725841,0.796921,0.695591,0.866613,0.456093,...
% %     0.766566,0.937647,0.765182,0.193738];
% % mccB = [0.929565,0.854946,0.978619,0.172896,0.199395,0.898843,0.719945,0.975287,0.971480,0.362275,...
% %     0.872254,0.818206,0.108974,0.660146,0.432523,0.801733,0.704143,0.853331,0.655364,0.940397,0.136162,...
% %     0.759217,0.915538,0.735852,0.136046];
% % mccR = [0.940780,0.853529,0.944652,0.897953,0.892121,0.879705,0.832668,0.982674,0.979179,0.995902,...
% %     0.846315,0.820421,0.619385,0.663360,0.842398,0.944220,0.722288,0.943464,0.794909,0.922984,0.969032,...
% %     0.792940,0.979741,0.820244,0.882349];
% % mccS = [0.940780,0.853529,0.944652,0.898056,0.892237,0.879705,0.832668,0.982674,0.979179,0.995902,...
% %     0.846315,0.820421,0.619385,0.663360,0.842398,0.944220,0.722288,0.943464,0.794909,0.922984,0.969032,...
% %     0.792940,0.979741,0.820244,0.882461];
% % 
% % 
% % imageset = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25];
% % figure
% % plot(preE,recE,'xr',...
% %     preB,recB,'xb',...
% %     preR,recR,'+k',...
% %     preS,recS,'xg');
% % xlabel('Precision');
% % ylabel('Recall');
% % rect = [0.25 0.175 0.2 0.1]; % position of legend
% % l=legend('Eriksson et al.','Boykov et al.','Proposed ','Proposed with hard constraints');
% % set(l,'Position',rect);
% % 
% % figure
% % plot(imageset,accE,'- .r',...
% %     imageset,accB,'- .b',...
% %     imageset,accR,'- +k',...
% %     imageset,accS,'- .g');
% % xlabel('Image');
% % ylabel('Accuracy');
% % rect = [0.25 0.175 0.2 0.1]; % position of legend
% % l=legend('Eriksson et al.','Boykov et al.','Proposed ','Proposed with hard constraints');
% % set(l,'Position',rect);
% % 
% % fprintf('\n\nStats\t\t\t\tMean\t\tStd. Dev\n');
% % fprintf('Eriksson(Precision)\t%f\t%f\n',   mean(preE),std(preE));
% % fprintf('Eriksson(Recall)\t\t%f\t%f\n',    mean(recE),std(recE));
% % fprintf('Eriksson(Accuracy)\t%f\t%f\n',  mean(accE),std(accE));
% % fprintf('Eriksson(MCC)\t\t%f\t%f\n', mean(mccE),std(mccE));
% % 
% % fprintf('Boykov(Precision)\t%f\t%f\n',   mean(preB),std(preB));
% % fprintf('Boykov(Recall)\t\t%f\t%f\n',    mean(recB),std(recB));
% % fprintf('Boykov(Accuracy)\t\t%f\t%f\n',  mean(accB),std(accB));
% % fprintf('Boykov(MCC)\t\t\t%f\t\t\t%f\n', mean(mccB),std(mccB));
% % 
% % fprintf('Proposed(Precision)\t\t%f\t%f\n', mean(preR),std(preR));
% % fprintf('Proposed(Recall)\t\t%f\t%f\n',    mean(recR),std(recR));
% % fprintf('Proposed(Accuracy)\t\t%f\t%f\n',  mean(accR),std(accR));
% % fprintf('Proposed(MCC)\t\t\t%f\t\t\t%f\n', mean(mccR),std(mccR));
% % 
% % fprintf('Proposed with hard constraints(Precision)\t%f\t%f\n',  mean(preS),std(preS));
% % fprintf('Proposed with hard constraints(Recall)\t\t%f\t%f\n',   mean(recS),std(recS));
% % fprintf('Proposed with hard constraints(Accuracy)\t%f\t%f\n',   mean(accS),std(accS));
% % fprintf('Proposed with hard constraints(MCC)\t\t\t%f\t\t\t%f\n',mean(mccS),std(mccS));
% % % 
% % % fprintf('EMGMM(Precision)\t%f\t%f\n',  mean(preEMGMM),std(preEMGMM));
% % % fprintf('EMGMM(Recall)\t\t%f\t%f\n',   mean(recEMGMM),std(recEMGMM));
% % % fprintf('EMGMM(Accuracy)\t\t%f\t%f\n', mean(accEMGMM),std(accEMGMM));
% % % fprintf('EMGMM(MCC)\t\t\t%f\t\t\t%f\n',mean(mccEMGMM),std(mccEMGMM));
% % % 
% % % fprintf('Dilate(Precision)\t%f\t%f\n',  mean(preDilate),std(preDilate));
% % % fprintf('Dilate(Recall)\t\t%f\t%f\n',   mean(recDilate),std(recDilate));
% % % fprintf('Dilate(Accuracy)\t%f\t%f\n',   mean(accDilate),std(accDilate));
% % % fprintf('Dilate(MCC)\t\t\t%f\t\t\t%f\n',mean(mccDilate),std(mccDilate));





