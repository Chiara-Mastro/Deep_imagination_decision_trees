clear all
%close all

X_1 = load('Twob_1_optd.m');
X_n = load('Twob_n_optd.m');

X = NaN(numel(X_1(:,1))+numel(X_1(:,1)), numel(X_1(1,:)));

index=1;

for i=5:-1:0
    for j=1:9
        X(index,:) = X_1(9*i+j,:);
        index = index + 1;
    end
end

for i=1:numel(X_n(:,1))
    X(index,:) = X_n(i,:);
    index = index + 1;
end

C_vec = unique(X(:,5));
num_C = numel(C_vec);
p_vec = unique(X(:,6));
num_p = numel(p_vec);

Optimal_vec = NaN (num_p-1, num_C, 4);

index=1;

for i = 1:11
    for j = 1:9
        Optimal_vec(i,j,:) = X(index,1:4);
        index=index+1;
    end
end



figure
set(gcf, 'Units', 'Inches', 'Position', [0, 0, 5, 5], ...
    'PaperUnits', 'Inches', 'PaperSize', [5, 5])

pos1 = [0.15 0.60 0.3 0.35];
subplot('Position',pos1)
%mesh(p_vec(1:num_p-1), C_vec, Optimal_vec(:,:,1)', 'FaceColor', 'interp', 'FaceAlpha', '0.3');
mesh(p_vec(1:num_p-1), C_vec, Optimal_vec(:,:,1)');
ax=gca;
ax.FontSize=14;
set(gca, 'Yscale', 'log')
set(gca, 'Zscale', 'log')
zticks([10^0  10^2])
zticklabels({'10^0', '10^2'})
set(gca,'ZMinorGrid','off');
set(gca,'YMinorGrid','off');
yticks([10^1 10^3])
yticklabels({'10^1', '10^3'})
xlabel('$p$','Interpreter','Latex');
ylabel('$C$','Interpreter','Latex');
zlabel(' $V^*$','Interpreter','Latex')
box off
annotation('textbox', [0.05, 0.95, 0, 0], 'string', 'a', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')
view([-160 180 270])

pos2 = [0.65 0.60 0.3 0.35];
subplot('Position',pos2)
%mesh(p_vec(1:num_p-1), C_vec, Optimal_vec(:,:,2)', 'FaceColor', 'interp', 'FaceAlpha', '0.3');
h=mesh(p_vec(1:num_p-1), C_vec, Optimal_vec(:,:,2)');
ax=gca;
ax.FontSize=14;
set(gca, 'Yscale', 'log')
set(gca,'YMinorGrid','off');
xlabel('$p$','Interpreter','Latex');
ylabel('$C$','Interpreter','Latex');
yticks([10^1 10^3])
yticklabels({'10^1',  '10^3'})
zlabel(' $b_1^*$','Interpreter','Latex')
text(1.1, 1.5,1,'$\downarrow$', 'Interpreter', 'LaTeX')
text(1.1, 1.5,5.5,'$b_1^*=2$', 'Interpreter', 'LaTeX')
box off
annotation('textbox', [0.48, 0.95, 0, 0], 'string', 'b', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')
view([-160 180 270])

pos3 = [0.15 0.1 0.3 0.35];
subplot('Position',pos3)
%mesh(p_vec(1:num_p-1), C_vec, Optimal_vec(:,:,3)', 'FaceColor', 'interp', 'FaceAlpha', '0.3');
mesh(p_vec(1:num_p-1), C_vec, Optimal_vec(:,:,3)');
ax=gca;
ax.FontSize=14;
set(gca, 'Yscale', 'log')
set(gca,'YMinorGrid','off');
yticks([10^1 10^3])
yticklabels({'10^1', '10^3'})
xlabel('$p$','Interpreter','Latex');
ylabel('$C$','Interpreter','Latex');
zlabel(' $b_2^*$','Interpreter','Latex')
box off
text(1.1, 350,1,'$\downarrow$', 'Interpreter', 'LaTeX')
text(1.1, 350,4,'$b_2^*=1$', 'Interpreter', 'LaTeX')
annotation('textbox', [0.05, 0.45, 0, 0], 'string', 'c', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')
view([-160 180 270])

pos4 = [0.65 0.1 0.3 0.35];
subplot('Position',pos4)
%mesh(p_vec(1:num_p-1), C_vec, Optimal_vec(:,:,4)', 'FaceColor', 'interp', 'FaceAlpha', '0.3');
mesh(p_vec(1:num_p-1), C_vec, Optimal_vec(:,:,4)');
ax=gca;
ax.FontSize=14;
set(gca, 'Yscale', 'log')
set(gca,'YMinorGrid','off');
yticks([10^1 10^3])
yticklabels({'10^1', '10^3'})
xlabel('$p$','Interpreter','Latex');
ylabel('$C$','Interpreter','Latex');
zlabel(' $d_1^*$','Interpreter','Latex')
box off
text(1.1, 1.5,1,'$\downarrow$', 'Interpreter', 'LaTeX')
text(1.1, 1.5,2,'$d_1^*=1$', 'Interpreter', 'LaTeX')
annotation('textbox', [0.48, 0.45, 0, 0], 'string', 'd', 'FontName', 'Times', 'FontSize',22, 'FontWeight','bold')
view([-160 180 270])


%print pdf
print(gcf,'fig7','-dpdf','-r1000')