#include "pch.h"
//���д����ʵ��
#include "gene.h"
#include<bitset>
#include<iostream>
using namespace std;

Gene::Gene()
{
	cout << "begin" << endl;

}

Gene::~Gene()
{
}

int Gene::calSum(UINT *ch, int *pt)   //chΪװ�뱳���е�һ�����ܵĽ�  ptΪ�������������ָ��
{
	int popSum = 0;
	for (int i = 0; i < CHROM_SIZE; i++) {
		popSum += (*ch) * pt[i];
		ch++;
	}
	return popSum;
}

void Gene::initPop()
{
	int tmpWeight = 0;
	int tmpVolume = 0;
	int m = 0;
	bool isPop = false;
	//���������Ⱥ�ĳ�ʼ��
	for (int i = 0; i < POP_SIZE; i++) { //�����POP_SIZE����Ⱥ��ģ
		while (!isPop) {
			for (int j = 0; j < CHROM_SIZE; j++) {
				m = rand() % 1001;   //randΪ��ʼ��������������������0�ĸ���Ҫ��һЩ
				if (m <= 499) oldPop[i].chrom[j] = 0;
				else oldPop[i].chrom[j] = 1;
				oldPop[i].parent1 = 0;
				oldPop[i].parent2 = 0;
				oldPop[i].cross = 0;
			}
			//�޳�������������ڱ�������������ĸ���
			tmpWeight = calSum(oldPop[i].chrom, weight);
			tmpVolume = calSum(oldPop[i].chrom, volume);

			if ((tmpWeight <= containW) && (tmpVolume <= containV)) {
				oldPop[i].fitness = calSum(oldPop[i].chrom, profit);
				oldPop[i].weight = tmpWeight;
				oldPop[i].volume = tmpVolume;
				oldPop[i].parent1 = 0;
				oldPop[i].parent2 = 0;
				oldPop[i].cross = 0;
				isPop = true;
			}
		}
		isPop = false;
	}

}

void Gene::statistics(struct population *pop)
{
	double tmpFitness;
	minPop = 0;
	maxPop = 0;

	sumFitness = pop[0].fitness;
	minFitness = pop[0].fitness;
	maxFitness = pop[0].fitness;

	for (int i = 1; i < POP_SIZE; i++) {
		sumFitness += pop[i].fitness;
		tmpFitness = pop[i].fitness;

		//��ѡ��������Ӧ�ȸ���
		if ((tmpFitness > maxFitness) && ((int)(tmpFitness * 10) % 10 == 0)) {
			maxFitness = pop[i].fitness;
			maxPop = i;
		}
		//��ѡ����С����Ӧ�ȸ���
		if (tmpFitness < minFitness) {
			minFitness = pop[i].fitness;
			minPop = i;
		}
		//�����ƽ������Ӧ��
		avgFitness = sumFitness / (float)POP_SIZE;
	}
}

void Gene::report(struct population *pop, int gen,int thread_num)
{
	int popWeight = 0;
	cout << "======================================" << endl;
	cout << "The generation is " << gen << " from thread "<<thread_num<<endl;  //��ʾ��Ⱥ�Ĵ���
	cout << "The population chrom is: " << endl;
	for (int j = 0; j < CHROM_SIZE; j++) {
		if (j % 4 == 0) cout << " ";
		cout << pop[maxPop].chrom[j];
	}
	cout << endl;
	cout << "The population's max fitness is: " << (int)pop[maxPop].fitness << endl;
	cout << "The population's max weight is: " << (int)pop[minPop].weight << endl;
	cout << "The population's max volume is: " << (int)pop[minPop].weight << endl;
	cout << "========================================" << endl;
}

int Gene::selection(int pop)   //ʹ���ֶķ�����ѡ��
{
	double wheelPos, randNumber, partsum = 0;
	int i = 0;
	randNumber = (rand() % 2001) / 2000.0;
	wheelPos = randNumber * sumFitness;
	do
	{
		partsum += oldPop[i].fitness;
		i++;
	} while ((partsum < wheelPos) && (i < POP_SIZE));
	return i - 1;
}

int Gene::crossOver(UINT *parent1, UINT *parent2, int i)
{
	int j;         //������Ļ���λ��
	int crossPos;  //������λ��
	if (excise(RRO_CROSS)) { crossPos = rand() % (CHROM_SIZE - 1); }
	else { crossPos = CHROM_SIZE - 1; }
	for (j = 0; j <= crossPos; j++) { newPop[i].chrom[j] = parent1[j]; }
	for (j = crossPos + 1; j < CHROM_SIZE; j++) { newPop[i].chrom[j] = parent2[j]; }
	newPop[i].cross = crossPos;
	return 1;

}

int Gene::excise(double probability)   //������ʲ���������ѡ��ʵ��
{
	double pp;
	pp = (double)(rand() % 20001 / 20000.0);
	if (pp <= probability) { return 1; }
	else { return 0; }
}

int Gene::mutation(UINT alleles)
{
	if (excise(PRO_MUTATE)) {
		alleles == 0 ? alleles = 1 : alleles = 0;
	}
	return alleles;
}


void Gene::generation()
{
	UINT mate1, mate2;
	UINT i, j;
	int tmpWeight = 0;
	int tmpVolume = 0;
	bool notGen;
	for (i = 0; i < POP_SIZE; i++) {
		notGen = false;
		while (!notGen) {
			mate1 = selection(i);  //ѡ���м��ʲ������������˫�׵�λ��
			mate2 = selection(i + 1);
			crossOver(oldPop[mate1].chrom, oldPop[mate2].chrom, i);
			for (j = 0; j < CHROM_SIZE; j++) {
				newPop[i].chrom[j] = mutation(newPop[i].chrom[j]); //���������ĸ���
			}
			tmpWeight = calSum(newPop[i].chrom, weight);
			tmpVolume = calSum(newPop[i].chrom, volume);
			if ((tmpWeight <= containW) && (tmpVolume <= containV)) {
				newPop[i].fitness = calSum(newPop[i].chrom, profit);
				newPop[i].weight = tmpWeight;
				newPop[i].volume = tmpVolume;
				newPop[i].parent1 = mate1;
				newPop[i].parent2 = mate2;
				notGen = true;
			}
		}
	}
}
