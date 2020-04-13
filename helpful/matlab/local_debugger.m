coord = [52 52 1];
angle = deg2rad(-45);
mv1 = [1 0 0; 0 1 0; -62 -62 1];
rtm = [cos(angle) sin(angle) 0; -sin(angle) cos(angle) 0; 0 0 1];
mv2 = [1 0 0; 0 1 0; 62 62 1];

coord*mv1*rtm*mv2