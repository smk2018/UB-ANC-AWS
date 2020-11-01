clear all;
    for i = 1:5
      x_target(i) = 5+rand(1);
      y_target(i) = 5+rand(1);
      x = rand(1);
      y = rand(1);
      X(i) = x;
      Y(i) = y;
    end
  
  fid = figure;
  hold on
  writerObj = VideoWriter('out.avi'); 
  writerObj.FrameRate = 2; 
  open(writerObj); 
  e = 0;
  

 while sqrt((X(1)-x_target(1))^2+(Y(1)-y_target(1))^2)>0.8 ||  sqrt((X(2)-x_target(2))^2+(Y(2)-y_target(2))^2)>0.8 || sqrt((X(3)-x_target(3))^2+(Y(3)-y_target(3))^2)>0.8 || sqrt((X(4)-x_target(4))^2+(Y(4)-y_target(4))^2)>0.8 || sqrt((X(5)-x_target(5))^2+(Y(5)-y_target(5))^2)>0.8
      e = e + 1;
    
      pause(0.5);
      figure(fid);
       
      %figure; 
     for j = 1:5
       theta = 0:1:360;
       Circle1=X(j)+1*cosd(theta);
       Circle2=Y(j)+1*sind(theta);
       plot(Circle1,Circle2,'black','linewidth',1);
       axis equal
       hold on;
     end
    
    r = 1;
    plot(x_target(1:5),y_target(1:5),'+','markersize',10);
    text(x_target(1),y_target(1),'target1','FontSize',12);
    text(x_target(2),y_target(2),'target2','FontSize',12);
    text(x_target(3),y_target(3),'target3','FontSize',12);
    text(x_target(4),y_target(4),'target4','FontSize',12);
    text(x_target(5),y_target(5),'target5','FontSize',12);
    hold on
    
    plot(X(1:5),Y(1:5),'b+','markersize',10);
    text(X(1),Y(1),'node1','FontSize',12);
    text(X(2),Y(2),'node2','FontSize',12);
    text(X(3),Y(3),'node3','FontSize',12);
    text(X(4),Y(4),'node4','FontSize',12);
    text(X(5),Y(5),'node5','FontSize',12);
        
    %roots of each two circles
    
    for i = 1:5    
        
       angle(i) = double(atand((y_target(i) - Y(i))/(x_target(i) - X(i))));
       
       if(x_target(i) - X(i) >= 0)
           if(y_target(i) - Y(i) >= 0)         %1
               x_angle(i) = angle(i);
               y_angle(i) = angle(i);
           else                                %4
               x_angle(i) = 360 + angle(i);
               y_angle(i) = 360 + angle(i);
           end
       else
           if(y_target(i) - Y(i) >= 0)         %2
               x_angle(i) = 180 + angle(i);
               y_angle(i) = 180 + angle(i);
           else                                %3
               x_angle(i) = 180 + angle(i);
               y_angle(i) = 180 + angle(i);
           end
       end

       p_x(i,e) = X(i) + 1*cosd(x_angle(i));
       p_y(i,e) = Y(i) + 1*sind(y_angle(i));
        
    end
   
   
              
    for i = 1:5
      plot(p_x(i,e),p_y(i,e),'r+','markersize',10);
      hold on;
    end

    for i = 1:5
      for u = 1:5
         r(i,u) = sqrt((p_x(i,e)-p_x(u,e))^2+(p_y(i,e)-p_y(u,e))^2);
      end
         x_original(i) = X(i);
         y_original(i) = Y(i);
      if(r(i,:) < 1.0)
          X(i) = p_x(i,e);
          Y(i) = p_y(i,e);
      else
          X(i) = X(i) + 0.5*(p_x(i,e)-X(i));
          Y(i) = Y(i) + 0.5*(p_y(i,e)-Y(i));
      end

    end
    
    check = 0;
    for i = 1:5
        if(sqrt((X(i)-x_target(i))^2+(Y(i)-y_target(i))^2) < 0.8)
            check = check + 1;
        end
    end
    
    if(check >= 4)
        break;
    end

    frame = getframe(gcf); 
    writeVideo(writerObj, frame);
    hold off
 end
    
 hold off
 close(writerObj); 