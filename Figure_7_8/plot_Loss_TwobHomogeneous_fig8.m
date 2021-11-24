clear all
close all

Lossb1b2d1_1 = load('Twob_1_loss_twob.m');
Lossb1b2_1 = load('Twob_1_loss_oneb.m');
Lossb1b2d1_n = load('Twob_n_loss_twob.m');
Lossb1b2_n = load('Twob_n_loss_twob.m');

Lossb1b2d1 = NaN(numel(Lossb1b2d1_1(:,1))+numel(Lossb1b2d1_n(:,1)), numel(Lossb1b2d1_1(1,:)));
Lossb1b2 = NaN(numel(Lossb1b2_1(:,1))+numel(Lossb1b2_n(:,1)), numel(Lossb1b2_1(1,:)));

index=1;

for i=5:-1:0
    for j=1:9
        Lossb1b2d1(index,:) = Lossb1b2d1_1(9*i+j,:);
        Lossb1b2(index,:) = Lossb1b2_1(9*i+j,:);
        index = index + 1;
    end
end

for i=1:numel(Lossb1b2d1_n(:,1))
    Lossb1b2d1(index,:) = Lossb1b2d1_n(i,:);    
    Lossb1b2(index,:) = Lossb1b2_n(i,:);
    index = index + 1;
end

C_vec = unique(Lossb1b2d1(:,9));
num_C = numel(C_vec);
p_vec = unique(Lossb1b2d1(:,10));
num_p = numel(p_vec);

Loss_vec = NaN (num_p, num_C, 2);

index=1;

for i = 1:num_p
    for j = 1:num_C
        Loss_vec(i,j,2) = 100*(Lossb1b2d1(index,2)-Lossb1b2d1(index,1))/Lossb1b2d1(index,2);
        Loss_vec(i,j,1) = 100*(Lossb1b2(index,2)-Lossb1b2(index,1))/Lossb1b2(index,2);
        index=index+1;
    end
end

% 

figure

set(gcf, 'Units', 'Inches', 'Position', [0, 0.5, 6.5, 3.5], ...
    'PaperUnits', 'Inches', 'PaperSize', [6.5, 3.5])

pos1 = [0.15 0.2 0.32 0.62];
pos2 = [0.65 0.20 0.32 0.62];
subplot('Position',pos2)
%mesh(p_vec, C_vec, Loss_vec(:,:,1)', 'FaceColor', 'interp', 'FaceAlpha', '0.3');
mesh(p_vec, C_vec, Loss_vec(:,:,1)');
ax=gca;
ax.FontSize=14;
set(gca, 'Yscale', 'log')
xlabel('$p$','Interpreter','Latex');
ylabel('$C$','Interpreter','Latex');
zlabel('Loss','Interpreter','Latex')
yticks([10^0 10^2 10^3])
yticklabels({'10^0', '10^2', '10^3'})
zticks([10 50 100])
zticklabels({'10%', '50%', '100%'})
zlim([0 100])
box off
annotation('textbox', [0.05, 0.95, 0, 0], 'string', 'a', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')
text(0.05,0.84,'$b_1=b_2=2$','Units','normalized','Color',[0 0 0],'FontSize',14,...
     'FontName','Times New Roman','Interpreter','Latex');
% view([-160 180 270])
 
subplot('Position',pos1)
%mesh(p_vec, C_vec, Loss_vec(:,:,2)', 'FaceColor', 'interp', 'FaceAlpha', '0.3');
mesh(p_vec, C_vec, Loss_vec(:,:,2)');
ax=gca;
ax.FontSize=14;
set(gca, 'Yscale', 'log')
xlabel('$p$','Interpreter','Latex');
ylabel('$C$','Interpreter','Latex');
zlabel(' Loss','Interpreter','Latex');
zticks([10 50 100])
zticklabels({'10%', '50%', '100%'})
zlim([0 100])
yticks([10^0 10^2 10^3])
yticklabels({'10^0', '10^2', '10^3'})
box off
annotation('textbox', [0.5, 0.95, 0, 0], 'string', 'b', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')
text(0.05,0.84,'$b_1=2, b_2=1, d_1=1$','Units','normalized','Color',[0 0 0],'FontSize',14,...
     'FontName','Times New Roman','Interpreter','Latex');
% view([-160 180 270])


%print pdf
print(gcf,'fig8','-dpdf','-r1000')
