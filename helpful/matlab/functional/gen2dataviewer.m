%datadir = '../24082019_0113/';
datadir = '../24082019_1728/';


DTDMPCluster = struct;
DTDMPCluster.rhomboid = gen2grparser(datadir, 3, 'rhomboid');
DTDMPCluster.sector = gen2grparser(datadir, 3, 'sector');
DTDMPCluster.snake = gen2grparser(datadir, 3, 'snake');

%figure;
%gen2rsdrawer(DTDMPCluster.rhomboid, DTDMPCluster.sector, DTDMPCluster.snake, 'm');

figure;
gen2rsdrawer_spec(DTDMPCluster.rhomboid, 'Стратегия 1');
figure;
gen2rsdrawer_spec(DTDMPCluster.sector, 'Стратегия 2');
figure;
gen2rsdrawer_spec(DTDMPCluster.snake, 'Стратегия 3');
