

%plots stats of the trajectories

clear;
%clf;

%p=0.01%
X1 = load('value_actions_capacity4.m'); %%low capacity
X1_th = load('value_actions_theory_capacity4.m');
X2 = load('value_actions_capacity5.m');  %%medium capacity
X2_th = load('value_actions_theory_capacity5.m');
X3 = load('value_actions_capacity6.m'); %%large capacity
X3_th = load('value_actions_theory_capacity6.m');

%color palette
num_C = 3;
for i=1:num_C
    color(:,i) = [i/num_C  0  (num_C-i)/num_C];
    %color(:,i) = [(i-1)/num_C  (i-1)/num_C  (i-1)/num_C];
end

figure(5)
set(gcf, 'Units', 'Inches', 'Position', [0, 0, 5, 5], ...
    'PaperUnits', 'Inches', 'PaperSize', [5, 5])

pos1 = [0.15 0.65 0.3 0.3];
subplot('Position',pos1)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
plot(X1(:,1),X1(:,3),'.','Color',color(:,1));
plot(X2(:,1),X2(:,3),'.','Color',color(:,2));
plot(X3(:,1),X3(:,3),'.','Color',color(:,3));
plot(X1_th(:,1),X1_th(:,3),'-','Color',color(:,1));
plot(X2_th(:,1),X2_th(:,3),'-','Color',color(:,2));
plot(X3_th(:,1),X3_th(:,3),'-','Color',color(:,3));
xlabel('$b$','Interpreter','Latex');
ylabel('value')
xlim([1 30])
ylim([0 1.5])
xticks([1 5 10 20 30])
text(0.7,0.90,'$p=0.01$','Units','normalized','Color',[0 0 0],'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');

pos2 = [0.6 0.65 0.3 0.3];
subplot('Position',pos2)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
set(gca, 'XScale', 'log')
plot(X1(:,2),X1(:,3),'.','Color',color(:,1));
plot(X2(:,2),X2(:,3),'.','Color',color(:,2));
plot(X3(:,2),X3(:,3),'.','Color',color(:,3));
plot(X1_th(:,2),X1_th(:,3),'-','Color',color(:,1));
plot(X2_th(:,2),X2_th(:,3),'-','Color',color(:,2));
plot(X3_th(:,2),X3_th(:,3),'-','Color',color(:,3));
xlabel('$d$','Interpreter','Latex');
%ylabel('value')
xlim([0 1000])
ylim([-0.01 1.5])
xticks([1 2 5 10 1e2 1e3])
text(0.7,0.90,'$p=0.01$','Units','normalized','Color',[0 0 0],'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');

pos3 = [0.15 0.2 0.3 0.3];
subplot('Position',pos3)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')
set(gca, 'YScale', 'log')
plot(X1(:,1),X1(:,2),'-','Color',color(:,1));
plot(X2(:,1),X2(:,2),'-','Color',color(:,2));
plot(X3(:,1),X3(:,2),'-','Color',color(:,3));
xlabel('sampled branches, $b$','Interpreter','Latex');
ylabel('sampled layers, $d$','Interpreter','Latex')
xticks([1 5 10 20 30])
yticks([1 2 5 10 1e2 1e3])
text(0.7,0.74,'$C=10$','Units','normalized','Color',color(:,1),'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.7,0.82,'$C=100$','Units','normalized','Color',color(:,2),'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');
text(0.7,0.90,'$C=1000$','Units','normalized','Color',color(:,3),'FontSize',8,...
    'FontName','Times New Roman','Interpreter','Latex');

text(2,20,'$\leftarrow$', 'Interpreter', 'LaTeX')
text(5,21,'depth', 'FontName','Times New Roman')
text(25,5,'$\downarrow$', 'Interpreter', 'LaTeX')
text(22,10,'breadth', 'FontName','Times New Roman')


pos4 = [0.6 0.2 0.3 0.3];
subplot('Position',pos4)
hold on
set(gca,'fontsize',11)
set(gca, 'FontName', 'Times New Roman')


%print pdf
print('fig3_p_0_01','-dpdf')

