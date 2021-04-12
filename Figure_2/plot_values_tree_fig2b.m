%Value as a function of the depth of the trees for different values of the probability of 
% positive reward p=0.10,0.50,0.75 with fixed branching factor b=2


%plots stats of the trajectories

clear;

X1 = load('value_actions_p0_1.m');
X2 = load('value_actions_p0_5.m');
X3 = load('value_actions_p0_75.m');
X_th1 = load('value_actions_theory_p0_1.m');
X_th2 = load('value_actions_theory_p0_5.m');
X_th3 = load('value_actions_theory_p0_75.m');

figure(5)
set(gcf, 'Units', 'Inches', 'Position', [0, 0.5, 5, 5], ...
    'PaperUnits', 'Inches', 'PaperSize', [5, 5])

pos2 = [0.65 0.60 0.32 0.32];
subplot('Position',pos2)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
plot(X_th1(:,1),X_th1(:,2),'Color',[0 0 1]); 
plot(X_th2(:,1),X_th2(:,2),'Color',[1 0 0]); 
plot(X_th3(:,1),X_th3(:,2),'Color',[0 0.5 0]); 
plot(X1(:,1),X1(:,2),'.','MarkerSize',10,'Color',[0 0 1]); 
plot(X2(:,1),X2(:,2),'.','MarkerSize',10,'Color',[1 0 0]); 
plot(X3(:,1),X3(:,2),'.','MarkerSize',10,'Color',[0 0.5 0]); 
u=[0:0.1:max(X_th2(:,1))];
plot(u,u,'k--');
xlabel('depth, $d$','Interpreter','Latex');
ylabel('value, $V_{d,b}$','Interpreter','Latex')

text(0.3,1.00,'$p=0.75$','Units','normalized','Color',[0 0.5 0],'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.3,0.92,'$p=0.5$','Units','normalized','Color',[1 0 0],'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.3,0.84,'$p=0.1$','Units','normalized','Color',[0 0 1],'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.1,0.92,'$b=2$','Units','normalized','Color',[0 0 0],'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');

annotation('textbox', [0.5, 1, 0, 0], 'string', 'b', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')

%print pdf
print('fig','-dpdf')

