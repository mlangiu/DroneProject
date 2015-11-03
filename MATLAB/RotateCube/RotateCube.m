
function cubeOut = RotateCube(angle, rotationAxis, cube)
    if cube == 0
        rotationAxis = [1,1,1];
        rotationAxis = rotationAxis/norm(rotationAxis);
        q = [cos(angle/2), rotationAxis(1) * sin(angle/2), rotationAxis(2) * sin(angle/2), rotationAxis(3) * sin(angle/2)];
        faces = [1 2 3 4; 2 6 7 3; 4 3 7 8; 1 5 8 4; 1 2 6 5; 5 6 7 8];
        vertices = (Matfromq(q)*[-0.5 -0.5 -0.5; -0.5 0.5 -0.5; 0.5 0.5 -0.5; 0.5 -0.5 -0.5; -0.5 -0.5 0.5; -0.5 0.5 0.5; 0.5 0.5 0.5; 0.5 -0.5 0.5]')';
        cubeOut = patch('Vertices',vertices,'Faces',faces,'FaceAlpha',0.8,'FaceColor','flat','FaceVertexCData',hsv(6),'erasemode','xor');
        view(3);
        axis square;
        axis([-2 2 -2 2 -2 2]);
        xlabel('X');
        ylabel('Y');
        zlabel('Z');
        grid on;
    else
        rotationAxis = [1,1,1];
        rotationAxis = rotationAxis/norm(rotationAxis);
        q = [cos(angle/2), rotationAxis(1) * sin(angle/2), rotationAxis(2) * sin(angle/2), rotationAxis(3) * sin(angle/2)];
        cube.Vertices = (Matfromq(q)*[-0.5 -0.5 -0.5; -0.5 0.5 -0.5; 0.5 0.5 -0.5; 0.5 -0.5 -0.5; -0.5 -0.5 0.5; -0.5 0.5 0.5; 0.5 0.5 0.5; 0.5 -0.5 0.5]')';
        drawnow update;
        cubeOut = cube;
    end
end

% function q3= quadMult(q1,q2)
%      q3 = zeros(4,1);
%      q3(1) = q1(1)*q2(1) - q1(2)*q2(2) - q1(3)*q2(3) - q1(4)*q2(4);
%      q3(2) = q1(1)*q2(2) + q1(2)*q2(1) + q1(3)*q2(4) - q1(4)*q2(3);
%      q3(3) = q1(1)*q2(3) + q1(3)*q2(1) + q1(2)*q2(4) - q1(4)*q2(2);
%      q3(4) = q1(1)*q2(4) + q1(4)*q2(1) + q1(2)*q2(3) - q1(3)*q2(2);
% end

function m = Matfromq(q)
     m = zeros(3,3);
     xSq = q(2) * q(2);
     ySq = q(3) * q(3);
     zSq = q(4) * q(4);
     wSq = q(1) * q(1);
     twoX = 2 * q(2);
     twoY = 2 * q(3);
     twoW = 2 * q(1);
     xy = twoX * q(3);
     xz = twoX * q(4);
     yz = twoY * q(4);
     wx = twoW * q(2);
     wy = twoW * q(3);
     wz = twoW * q(4);
     %fill in the first row
     m(1,1) = wSq + xSq - ySq - zSq;
     m(1,2) = xy - wz;
     m(1,3) = xz + wy;
     %fill in the second row
     m(2,1) = xy + wz;
     m(2,2) = wSq - xSq + ySq - zSq;
     m(2,3) = yz - wx;
     %fill in the third row
     m(3,1) = xz - wy;
     m(3,2) = yz + wx;
     m(3,3) = wSq - xSq - ySq + zSq;
end