threads = [1,2,4,8];
seq = [1.000000000,0.99550949,0.99694442,1.0053616];
openMP =  [0.9593355, 1.8347298, 3.42612622, 4.60378157];
pThreads = [1.03152071,1.94636998,3.64018744,4.95625095];

figure();
hold on;
semilogx(threads,seq, LineWidth=2);
semilogx(threads, openMP, '--', LineWidth=2);
semilogx(threads, pThreads, '-.', LineWidth=2);
legend('Sequentual', 'OpenMI', 'Pthreads','Location','northwest');
title('Parallel Speedup for Differing Threads');
xlabel('Number of Threads');
ylabel('Speedup');
hold off;
