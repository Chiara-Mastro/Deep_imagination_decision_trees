

%plots stats of the trajectories

clear;
%clf;

X = load('value_actions_capacity_loop2_data.m');

C_vec = unique(X(:,1));
p_vec = unique(X(:,2));

num_C = length(C_vec);
num_p = length(p_vec);

for i=1:num_C
    
    for j=1:num_p

        index = find(X(:,1)==C_vec(i) & X(:,2)==p_vec(j) );
        value_max(i,j) = max( X(index,5) );
        index_max = find( X(index,5) == max(X(index,5)) );
        index_max = min(index_max);
        b_opt(i,j) = X(index(index_max),3); 
        
        %depth heuristic, b=2
        index2 = find(X(:,1)==C_vec(i) & X(:,2)==p_vec(j) & X(:,3)==2 );
        loss_play_depth(i,j) = 100*(max( X(index,5) )-X(index2,5))/max( X(index,5) );
        
        %breadth heuristic, b=20
        index3 = find(X(:,1)==C_vec(i) & X(:,2)==p_vec(j) & X(:,3)==10 );
        loss_play_breadth(i,j) = 100*(max( X(index,5) )-X(index3,5))/max( X(index,5) );
                
    end
    
end



figure(5)
set(gcf, 'Units', 'Inches', 'Position', [0, 0, 5, 5], ...
    'PaperUnits', 'Inches', 'PaperSize', [5, 5])

pos1 = [0.15 0.60 0.3 0.35];
subplot('Position',pos1)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
set(gca, 'XScale', 'log')
mesh(C_vec,p_vec,value_max')
contour(C_vec,p_vec,value_max')
view(25,45)
grid on
xlabel('$C$', 'Interpreter', 'LaTeX')
ylabel('$p$', 'Interpreter', 'LaTeX')
zlabel('value')
ylim([0 1])
zlim([0 10])
xticks([10, 100, 1000])
yticks([0, 0.5, 1])
annotation('textbox', [0, 1, 0, 0], 'string', 'a', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')

pos2 = [0.65 0.60 0.3 0.35];
subplot('Position',pos2)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
set(gca, 'XScale', 'log')
mesh(C_vec,p_vec,b_opt')
contour(C_vec,p_vec,b_opt')
view(120,45)
grid on
xlabel('$C$', 'Interpreter', 'LaTeX')
ylabel('$p$', 'Interpreter', 'LaTeX')
zlabel('$b^*$, optimal', 'Interpreter', 'LaTeX')
ylim([0 1])
zlim([0 30])
xticks([10, 100, 1000])
yticks([0, 0.5, 1])
text(1,0.8,0,'$\downarrow$', 'Interpreter', 'LaTeX')
text(1,0.65,5,'$b^*=2$', 'Interpreter', 'LaTeX')
annotation('textbox', [0.5, 1, 0, 0], 'string', 'b', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')

pos3 = [0.15 0.1 0.3 0.35];
subplot('Position',pos3)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
set(gca, 'XScale', 'log')
mesh(C_vec,p_vec,loss_play_depth')
contour(C_vec,p_vec,loss_play_depth')
view(25,45)
grid on
xlabel('$C$', 'Interpreter', 'LaTeX')
ylabel('$p$', 'Interpreter', 'LaTeX')
zlabel('loss (%)')
ylim([0 1])
zlim([0 80])
xticks([10, 100, 1000])
yticks([0, 0.5, 1])
zticks([0, 40, 80])
text(500,1,10,'$\downarrow$', 'Interpreter', 'LaTeX')
text(150,1,20,'loss $=0$', 'Interpreter', 'LaTeX')
annotation('textbox', [0.0, 0.5, 0, 0], 'string', 'c', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')

pos4 = [0.65 0.1 0.3 0.35];
subplot('Position',pos4)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
set(gca, 'XScale', 'log')
mesh(C_vec,p_vec,loss_play_breadth')
contour(C_vec,p_vec,loss_play_breadth')
view(25,45)
grid on
xlabel('$C$', 'Interpreter', 'LaTeX')
ylabel('$p$', 'Interpreter', 'LaTeX')
zlabel('loss (%)')
ylim([0 1])
zlim([0 80])
xticks([10, 100, 1000])
yticks([0, 0.5, 1])
zticks([0, 40, 80])
annotation('textbox', [0.5, 0.5, 0, 0], 'string', 'd', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')


%print pdf
print(gcf,'fig','-dpdf','-r1000')

