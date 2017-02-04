function u=diffusion_scheme_3D_novel(u,Dxx,Dxy,Dxz,Dyy,Dyz,Dzz,dt,par)

if(nargin<9)
    par =[0.029180542350452,0.051009964656270,0.056702441663829,0.000006816925755, ...
          0.000008999487006,0.000009650996104,0.004751977565242,0.000082895902805, ...
          0.000000310453682,0.002474067467941,0.000007941958352,0.004974632072065, ...
          0.000174128474993,0.004854111687777,0.090614087403327,0.000116874531236, ...
          0.005034186669466,0.000373963590204,0.017456316783794,0.000008810909572, ...
          0.194390591201552,0.000005600928728,0.000003816386672,0.000003302536833, ...
          0.000014736927086,0.000000132136404,0.000005381620877,0.000003078916993, ...
          0.000005671182049,0.000004345622765,0.000004236931951,0.000002758996948, ...
          0.043827391390423];
end

a=par(1:3); b=par(4:21); c=par(22:33);
Mx=zeros(3,3,3);
Mx(:,:,1)=[a(1) a(2) a(1); 0 0 0; -a(1) -a(2) -a(1)];
Mx(:,:,2)=[a(2) a(3) a(2); 0 0 0; -a(2) -a(3) -a(2)];
Mx(:,:,3)=[a(1) a(2) a(1); 0 0 0; -a(1) -a(2) -a(1)];
Mzz=zeros(5,5,5);
Mzz(:,:,1) =[b(1) b(2) b(3) b(2) b(1);
             b(2) b(4) b(5) b(4) b(2);
             b(3) b(5) b(6) b(5) b(3);
             b(2) b(4) b(5) b(4) b(2);
             b(1) b(2) b(3) b(2) b(1)];
Mzz(:,:,2) =[b(7) b(8)  b(9)  b(8)  b(7);
             b(8) b(10) b(11) b(10) b(8);
             b(9) b(11) b(12) b(11) b(9);
             b(8) b(10) b(11) b(10) b(8);
             b(7) b(8)  b(9)  b(8)  b(7)];
Mzz(:,:,3) =[-b(13) -b(14) -b(15) -b(14) -b(13);
             -b(14) -b(16) -b(17) -b(16) -b(14);
             -b(15) -b(17) -b(18) -b(17) -b(15);
             -b(14) -b(16) -b(17) -b(16) -b(14);
             -b(13) -b(14) -b(15) -b(14) -b(13)];
Mzz(:,:,4)=Mzz(:,:,2);
Mzz(:,:,5)=Mzz(:,:,1);
Mxz=zeros(5,5,5);
Mxz(:,:,1) =[ c(1)  c(2)  c(3)  c(2)  c(1);
              c(4)  c(5)  c(6)  c(5)  c(4);
                0     0     0     0     0 ;
             -c(4) -c(5) -c(6) -c(5) -c(4);   
             -c(1) -c(2) -c(3) -c(2) -c(1)];
Mxz(:,:,2) =[ c(7)   c(8)   c(9)   c(8)   c(7);
              c(10)  c(11)  c(12)  c(11)  c(10);
                0      0      0      0      0 ;
             -c(10) -c(11) -c(12) -c(11) -c(10);   
             -c(7)  -c(8)  -c(9)  -c(8)  -c(7)];
Mxz(:,:,4)=-Mxz(:,:,2);
Mxz(:,:,5)=-Mxz(:,:,1);
Mxy= permute(Mxz,[1 3 2]); Myz=permute(Mxz,[2 3 1]);
My = permute(Mx, [2 1 3]); Mz= permute(Mx,[2 3 1]);
Mxx= permute(Mzz,[3 1 2]); Myy= permute(Mzz,[2 3 1]);

u=diffusion_scheme_3D_novel_getUpdate(single(u),single(Dxx),single(Dxy),single(Dxz),single(Dyy),single(Dyz),single(Dzz),single(Mx),single(My),single(Mz), single(Mxx), single(Myy), single(Mzz), single(Mxy), single(Mxz), single(Myz), single(dt) );
u(~isfinite(u))=0;
            
