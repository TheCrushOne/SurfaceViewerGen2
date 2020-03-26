
% В 2016а не работает...
%M = readmatrix('test.txt')

%filename = 'test.txt';
%filename = 'data.txt';
%datadir = '../31052019_1839_AT/';
%datadir = '../01062019_0509/';
%datadir = '../16082019_2303/';
datadir = '../01112019_2258/';
%data_filename = strcat(datadir, 'test.txt');
meta_filename = strcat(datadir, 'config.xml');
xmlmeta = xml2struct(meta_filename);

DTDMPCluster = struct;
DTDMPCluster.summary = gen1grparser(datadir, xmlmeta.config.summary_time);
DTDMPCluster.onerun = gen1grparser(datadir, xmlmeta.config.onerun_time);
DTDMPCluster.build = gen1grparser(datadir, xmlmeta.config.build_time);
DTDMPCluster.create = gen1grparser(datadir, xmlmeta.config.create_time);
DTDMPCluster.backfire = gen1grparser(datadir, xmlmeta.config.backfire_time);
DTDMPCluster.score = gen1grparser(datadir, xmlmeta.config.score_count);

%idx = abs(z-(-0.01))<10^-3 ;
%z(idx) = 0 ;
%waterfall(x', y', z');
%scatter (x,y,10,z,'filled')
%colorbar
%caxis ([-0.01 0])
%[X,Y] = size(A);
%z = peaks(25);


%figure;
%gen1grdrawer(2, 6, 1, DTDMPCluster.summary);
%gen1grdrawer(2, 6, 2, DTDMPCluster.onerun);
%gen1grdrawer(2, 6, 3, DTDMPCluster.build);
%gen1grdrawer(2, 6, 4, DTDMPCluster.create);
%gen1grdrawer(2, 6, 5, DTDMPCluster.backfire);
%gen1grdrawer(2, 6, 6, DTDMPCluster.score);

figure;
gen1rsdrawer(DTDMPCluster.summary, 's');
figure;
gen1rsdrawer(DTDMPCluster.summary, 'm');
%figure;
%
%waterfall(1:Y, 1:X, A);
%[X,Y,Z] = peaks(30);
%waterfall(X, Y, Z);
%waterfall(0:2:20, 0:5:100, zeros(10, 20));

%mesh(A)