%Value as a function of the depth of the trees for different branching
%factors b=2,3,5 with fixed value of the probability p=1/2


%plots stats of the trajectories

clear;

X1 = load('value_actions_b_2.m');
X2 = load('value_actions_b_3.m');
X3 = load('value_actions_b_5.m');
X_th1 = load('value_actions_theory_b_2.m');
X_th2 = load('value_actions_theory_b_3.m');
X_th3 = load('value_actions_theory_b_5.m');


figure(5)
set(gcf, 'Units', 'Inches', 'Position', [0, 0.5, 5, 5], ...
    'PaperUnits', 'Inches', 'PaperSize', [5, 5])

pos1 = [0.15 0.60 0.32 0.32];
subplot('Position',pos1)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
plot(X1(:,1),X1(:,2),'.','MarkerSize',10,'Color',[1 0 0]); 
plot(X2(:,1),X2(:,2),'.','MarkerSize',10,'Color',[1 0.5 0]); 
plot(X3(:,1),X3(:,2),'.','MarkerSize',10,'Color',[1 0 1]); 
plot(X_th1(:,1),X_th1(:,2),'Color',[1 0 0]); 
plot(X_th2(:,1),X_th2(:,2),'Color',[1 0.5 0]); 
plot(X_th3(:,1),X_th3(:,2),'Color',[1 0 1]); 
u=[0:0.1:10];
plot(u,u,'k--');
xlabel('depth, $d$','Interpreter','Latex');
ylabel('value, $V_{d,b}$','Interpreter','Latex')
xlim([0 10])

text(0.3,0.84,'$b=2$','Units','normalized','Color',[1 0 0],'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.3,0.92,'$b=3$','Units','normalized','Color',[1 0.5 0],'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.3,1.00,'$b=5$','Units','normalized','Color',[1 0 1],'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.05,0.92,'$p=0.5$','Units','normalized','Color',[0 0 0],'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');

annotation('textbox', [0, 1, 0, 0], 'string', 'a', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')

print('fig','-dpdf')

