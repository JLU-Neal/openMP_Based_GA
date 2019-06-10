// GA.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
//主函数入口
#include"gene.h"
#include <string.h>
#include <iostream>
#include<omp.h>
#include<time.h>
using namespace std;
#define NUMBER_OF_THREADS 4

population bestPop[NUMBER_OF_THREADS];
void findBest();
int main(int argc, char*argv[])
{
	clock_t start, finish;
	double totaltime;
	start = clock();



	omp_set_num_threads(NUMBER_OF_THREADS);
	
#pragma omp parallel
	{
		Gene *gene = new Gene;     //实例化类，返回指针
		int thread_num = omp_get_thread_num();
		int gen = 0;
		int oldMaxPop, k;
		double oldMax;

		srand((unsigned)time(NULL));
		gene->initPop();
		memcpy(&gene->newPop, &gene->oldPop, POP_SIZE * sizeof(struct population));
		gene->statistics(gene->newPop);    //计算种群的最大适应度和最小适应度以及适应度的下表号。
#pragma omp critical
		{
			gene->report(gene->newPop, gen,thread_num);
		}

		while (gen < CENERAION_NUM)
		{
			gen += 1;
			if (gen % 100 == 0) {
				srand((unsigned)time(NULL));
			}
			oldMax = gene->maxFitness;   //oldmax为种群中最大适应度
			oldMaxPop = gene->maxPop;    //oldMaxPop指种群中最大适应度的个体
			gene->generation();
			gene->statistics(gene->newPop);

			if (gene->maxFitness < oldMax) {
				for (k = 0; k < CHROM_SIZE; k++) {
					gene->newPop[gene->minPop].chrom[k] = gene->oldPop[oldMaxPop].chrom[k];
				}
				gene->newPop[gene->minPop].fitness = gene->oldPop[oldMaxPop].fitness;
				gene->newPop[gene->minPop].weight = gene->oldPop[oldMaxPop].weight;
				gene->newPop[gene->minPop].volume = gene->oldPop[oldMaxPop].volume;
				gene->newPop[gene->minPop].parent1 = gene->oldPop[oldMaxPop].parent1;
				gene->newPop[gene->minPop].parent2 = gene->oldPop[oldMaxPop].parent2;
				gene->newPop[gene->minPop].cross = gene->oldPop[oldMaxPop].cross;
				gene->statistics(gene->newPop);
			}
			else if (gene->maxFitness > oldMax) {
#pragma omp critical
				{
					gene->report(gene->newPop, gen,thread_num);
				}
				
			}
			//更新bestPop
			
			bestPop[thread_num] = gene->newPop[oldMaxPop];
			//============================

			//不同子种群之间的进行信息交流
			if (gene->maxFitness < bestPop[(thread_num + 1) % 4].fitness&&gen%10==0)//每隔十代进行交流
			{
				for (k = 0; k < CHROM_SIZE; k++) {
					gene->newPop[gene->minPop].chrom[k] = bestPop[(thread_num + 1) % 4].chrom[k];
				}
				gene->newPop[gene->minPop].fitness = bestPop[(thread_num + 1) % 4].fitness;
				gene->newPop[gene->minPop].weight = bestPop[(thread_num + 1) % 4].weight;
				gene->newPop[gene->minPop].volume = bestPop[(thread_num + 1) % 4].volume;
				gene->newPop[gene->minPop].parent1 = bestPop[(thread_num + 1) % 4].parent1;
				gene->newPop[gene->minPop].parent2 = bestPop[(thread_num + 1) % 4].parent2;
				gene->newPop[gene->minPop].cross = bestPop[(thread_num + 1) % 4].cross;
				gene->statistics(gene->newPop);
			}
			//=============================
			memcpy(&gene->oldPop, &gene->newPop, POP_SIZE * sizeof(struct population));
		}
		bestPop[thread_num] = gene->newPop[gene->maxPop];
		//delete[] gene;    //销毁对象占用空间
	}//并行部分结束
	findBest();
	
	//system("pause");
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "The openMP based Genetic Algorithm 's running time is " << totaltime << " sec ~" << endl;
	return 0;
}



void findBest()
{
	double bestFitnes = 0;
	int bestPosition = 0;
	for (int i = 0; i < NUMBER_OF_THREADS; i++)
	{
		if (bestPop[i].fitness > bestFitnes)
		{
			bestFitnes = bestPop[i].fitness;
			bestPosition = i;
		}

	}
	int popWeight = 0;
	cout << "The generation is 1000" << endl;  //显示种群的代数
	cout << "The population chrom is: " << endl;
	for (int j = 0; j < CHROM_SIZE; j++) {
		if (j % 4 == 0) cout << " ";
		cout << bestPop[bestPosition].chrom[j];
	}
	cout << endl;
	cout << "The population's max fitness is: " << (int)bestPop[bestPosition].fitness << endl;
	cout << "The population's max weight is: " << (int)bestPop[bestPosition].weight << endl;
	cout << "The population's max volume is: " << (int)bestPop[bestPosition].weight << endl;
}
