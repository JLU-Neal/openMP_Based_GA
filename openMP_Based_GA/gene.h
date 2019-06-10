/********
头文件的定义
********/

#pragma once
#ifndef GENE_H
#define GENE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define SUB_POP_SIZE 50//定义子种群规模
#define POP_SIZE 50 //定义种群规模
#define RRO_CROSS 0.618 //交叉概率
#define PRO_MUTATE 0.03 //变异概率
#define CHROM_SIZE 32  //给定染色体长度
#define CENERAION_NUM 1000 //定义繁殖代数
typedef unsigned int UINT;

struct population {      //定义私有的个体类
	UINT chrom[CHROM_SIZE];    //定义个体的基因组
	double weight;             //背包的重量
	double volume;             //背包的体积
	double fitness;            //个体的适应度
	UINT parent1, parent2, cross;   //双亲以及交叉的节点
};

class Gene
{
public:
	Gene();
	~Gene();
public:

	population oldPop[POP_SIZE], newPop[POP_SIZE];

	int weight[CHROM_SIZE] = { 22, 15, 4, 5, 10, 19, 21, 20, 8, 13, 2, 3, 3, 17, 12, 5, 12, 4, 1, 21, 14, 23, 17, 15, 20, 22, 25, 0, 22, 15, 25, 13 };
	int volume[CHROM_SIZE] = { 11, 22, 12, 21, 21, 13, 1, 10, 13, 8, 6, 25, 13, 27, 12, 23, 12, 24, 23, 11, 6, 24, 28, 10, 20, 13, 25, 23, 5, 26, 30, 15 };
	int profit[CHROM_SIZE] = { 8, 9, 15, 6, 16, 9, 1, 4, 14, 9, 3, 7, 12, 4, 15, 5, 18, 5, 15, 4, 6, 2, 12, 14, 11, 9, 13, 13, 14, 13, 19, 4 };
	int containW = 80, containV = 75;

	double sumFitness;   //种群总适应度
	double minFitness;   //最小适应度
	double maxFitness;   //最大适应度
	double avgFitness;   //平均适应度

	double alpha; //计算适应度时的惩罚系数

	int minPop;  //种群内最大和最小的适应个体
	int maxPop;

	void initPop();  //总群初始化函数
	//int calWeight(UINT *chr);  //计算个体体积，重量，以及收益的函数
	//int Gene::calVolume(UINT *chr);
	int calSum(UINT *ch, int *pt);
	double calFit(UINT *ch);
	void statistics(struct population *pop);  //计算种群最大适应度和最小适应度的函数
	void report(struct population *pop, int gen,int thread_num); //为输出的函数
	int selection(int pop);   //通过选择总群中符合要求的父母进行繁殖 函数返回父母的位置
	int crossOver(UINT *parent1, UINT *parent2, int i);   //传入要更改的个体位置，随机产生交叉位置
	int excise(double probability);// 传入概率参数，进行交叉或者变异
	int mutation(UINT i);  //传入参数为基因组基因的位置，逐个基因判断变异概率
	void generation();  //种群群体更新的函数
};


#endif // !GENE_H
