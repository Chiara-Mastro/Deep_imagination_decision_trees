

%plots stats of the trajectories

clear;
%clf;

X_q_dis= load('q_distribution_theory_loop4.m');
X_v_iter = load('value_iterations_theory_loop4.m');
X_v_iter_random = load('value_iterations_theory_loop5.m'); %we will take the value for random allocation here

C_vec = unique(X_v_iter(:,3));
b_vec = unique(X_v_iter(:,4));

C_r_vec = unique(X_v_iter_random(:,3));
b_r_vec = unique(X_v_iter_random(:,4));

num_C = length(C_vec);
num_b = length(b_vec);

%color palette
for i=1:num_C
    color(:,i) = [i/num_C  0  (num_C-i)/num_C];
    %color(:,i) = [(i-1)/num_C  (i-1)/num_C  (i-1)/num_C];
end

for i=1:num_C
    
    for j=1:num_b

        index = find(X_v_iter(:,3)==C_vec(i) & X_v_iter(:,4)==b_vec(j) );
        value(i,j) = max( X_v_iter(index,2) );
        value_ini(i,j) = max( X_v_iter(index,6) );
        b_mat(i,j) = b_vec(j);
        
        index_r = find(X_v_iter_random(:,3)==C_r_vec(i) & X_v_iter_random(:,4)==b_r_vec(j) );
        value_ini_random(i,j) = max( X_v_iter_random(index_r,6) );
                 
    end
    
end


figure(5)
set(gcf, 'Units', 'Inches', 'Position', [0, 0.5, 5, 5], ...
    'PaperUnits', 'Inches', 'PaperSize', [5, 5])

pos1 = [0.15 0.60 0.32 0.32];
subplot('Position',pos1)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
for i=1:num_C 
    for j=1:1 %we only plot b=2, the optimal policy
        index2 = find(X_q_dis(:,5)==C_vec(i) & X_q_dis(:,6)==b_vec(j) );
        q_dis = X_q_dis(index2,2);
        q_dis_x = X_q_dis(index2,1);
        
        plot(q_dis_x,q_dis,'Color',color(:,i)); 
        plot(q_dis_x,q_dis,'.','Color',color(:,i)); 
    end
end
xlabel('layer');
ylabel('optimal sampling probability')
xlim([1 20])
xticks([1 10 20])
yticks([0 0.5 1])
text(0.8,0.90,'$C=1000$','Units','normalized','Color',color(:,3),'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.8,0.82,'$C=100$','Units','normalized','Color',color(:,2),'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.8,0.74,'$C=10$','Units','normalized','Color',color(:,1),'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
annotation('textbox', [0, 1, 0, 0], 'string', 'a', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')

pos2 = [0.65 0.60 0.32 0.32];
subplot('Position',pos2)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
for i=1:num_C
    plot(b_mat(i,:),value(i,:),'Color',color(:,i)); 
    plot(b_mat(i,:),value(i,:),'.','Color',color(:,i)); 
    
    plot(b_mat(i,:),value_ini(i,:),'--','Color',color(:,i)); 
    plot(b_mat(i,:),value_ini(i,:),'.','Color',color(:,i)); 
end
xlabel('$b$','Interpreter','Latex');
ylabel('value')
xlim([2 10])
xticks([2 3 5 10])
yticks([0 4 8])
line([7 8],[7 7],'Color','k')
line([7 8],[6.5 6.5],'LineStyle','--','Color','k')
text(0.76,0.90,'heterog.','Units','normalized','FontSize',8,...
    'FontName','Times New Roman');
text(0.76,0.82,'homog.','Units','normalized','FontSize',8,...
    'FontName','Times New Roman');
annotation('textbox', [0.5, 1, 0, 0], 'string', 'b', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')

pos3 = [0.15 0.1 0.32 0.32];
subplot('Position',pos3)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
for i=1:num_C 
    for j=1:num_b
        index2 = find(X_q_dis(:,5)==C_vec(i) & X_q_dis(:,6)==b_vec(j) );
        q_dis = X_q_dis(index2,2);
        alfa_dis = X_q_dis(index2,4);
        q_dis_x = X_q_dis(index2,1);
        
        plot(q_dis_x,q_dis.*alfa_dis,'Color',color(:,i)); 
        plot(q_dis_x,q_dis.*alfa_dis,'.','Color',color(:,i));
    end
end
xlabel('layer');
ylabel('av. number of samples per depth')

pos4 = [0.65 0.1 0.32 0.32];
subplot('Position',pos4)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
set(gca, 'XScale', 'log')
plot(X_v_iter(:,1),X_v_iter(:,2)); 
xlabel('iterations');
ylabel('value')


figure(6)
set(gcf, 'Units', 'Inches', 'Position', [0, 0.5, 5, 2.5], ...
    'PaperUnits', 'Inches', 'PaperSize', [5, 2.5])

pos1 = [0.15 0.2 0.32 0.62];
subplot('Position',pos1)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
for i=1:num_C 
    for j=1:1 %we only plot b=2, the optimal policy
        index2 = find(X_q_dis(:,5)==C_vec(i) & X_q_dis(:,6)==b_vec(j) );
        q_dis = X_q_dis(index2,2);
        q_dis_x = X_q_dis(index2,1);
        
        plot(q_dis_x,q_dis,'Color',color(:,i)); 
        plot(q_dis_x,q_dis,'.','Color',color(:,i)); 
    end
end
xlabel('level');
ylabel('optimal sampling probability')
xlim([1 20])
xticks([1 10 20])
yticks([0 0.5 1])
text(0.8,0.90,'$C=1000$','Units','normalized','Color',color(:,3),'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.8,0.82,'$C=100$','Units','normalized','Color',color(:,2),'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.8,0.74,'$C=10$','Units','normalized','Color',color(:,1),'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
annotation('textbox', [0, 1, 0, 0], 'string', 'a', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')

pos2 = [0.65 0.20 0.32 0.62];
subplot('Position',pos2)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
for i=1:num_C
    plot(b_mat(i,:),value(i,:),'Color',color(:,i)); 
    plot(b_mat(i,:),value(i,:),'.','Color',color(:,i)); 
    
    plot(b_mat(i,:),value_ini(i,:),'--','Color',color(:,i)); 
    plot(b_mat(i,:),value_ini(i,:),'.','Color',color(:,i)); 
    
    plot(b_mat(i,:),value_ini_random(i,:),':','Color',color(:,i)); 
    plot(b_mat(i,:),value_ini_random(i,:),'.','Color',color(:,i)); 
end
xlabel('$b$','Interpreter','Latex');
ylabel('value')
xlim([2 10])
xticks([2 3 5 10])
yticks([0 4 8])
line([7 8],[7 7],'Color','k')
line([7 8],[6.5 6.5],'LineStyle','--','Color','k')
line([7 8],[6. 6.],'LineStyle',':','Color','k')
text(0.76,0.90,'heterog.','Units','normalized','FontSize',8,...
    'FontName','Times New Roman');
text(0.76,0.82,'homog.','Units','normalized','FontSize',8,...
    'FontName','Times New Roman');
text(0.76,0.74,'random','Units','normalized','FontSize',8,...
    'FontName','Times New Roman');
annotation('textbox', [0.5, 1, 0, 0], 'string', 'b', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')

%print pdf
print('fig','-dpdf')

