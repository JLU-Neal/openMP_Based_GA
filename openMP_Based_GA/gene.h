/********
ͷ�ļ��Ķ���
********/

#pragma once
#ifndef GENE_H
#define GENE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define SUB_POP_SIZE 50//��������Ⱥ��ģ
#define POP_SIZE 50 //������Ⱥ��ģ
#define RRO_CROSS 0.618 //�������
#define PRO_MUTATE 0.03 //�������
#define CHROM_SIZE 32  //����Ⱦɫ�峤��
#define CENERAION_NUM 1000 //���己ֳ����
typedef unsigned int UINT;

struct population {      //����˽�еĸ�����
	UINT chrom[CHROM_SIZE];    //�������Ļ�����
	double weight;             //����������
	double volume;             //���������
	double fitness;            //�������Ӧ��
	UINT parent1, parent2, cross;   //˫���Լ�����Ľڵ�
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

	double sumFitness;   //��Ⱥ����Ӧ��
	double minFitness;   //��С��Ӧ��
	double maxFitness;   //�����Ӧ��
	double avgFitness;   //ƽ����Ӧ��

	double alpha; //������Ӧ��ʱ�ĳͷ�ϵ��

	int minPop;  //��Ⱥ��������С����Ӧ����
	int maxPop;

	void initPop();  //��Ⱥ��ʼ������
	//int calWeight(UINT *chr);  //�������������������Լ�����ĺ���
	//int Gene::calVolume(UINT *chr);
	int calSum(UINT *ch, int *pt);
	double calFit(UINT *ch);
	void statistics(struct population *pop);  //������Ⱥ�����Ӧ�Ⱥ���С��Ӧ�ȵĺ���
	void report(struct population *pop, int gen,int thread_num); //Ϊ����ĺ���
	int selection(int pop);   //ͨ��ѡ����Ⱥ�з���Ҫ��ĸ�ĸ���з�ֳ �������ظ�ĸ��λ��
	int crossOver(UINT *parent1, UINT *parent2, int i);   //����Ҫ���ĵĸ���λ�ã������������λ��
	int excise(double probability);// ������ʲ��������н�����߱���
	int mutation(UINT i);  //�������Ϊ����������λ�ã���������жϱ������
	void generation();  //��ȺȺ����µĺ���
};


#endif // !GENE_H
