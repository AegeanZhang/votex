#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ITER 1000  // 最大迭代次数
#define LEARNING_RATE 0.1  // 学习率
#define NUM_INPUTS 2  // 输入特征的数量（这里假设有两个输入特征）

// 感知机结构体
typedef struct {
    double weights[NUM_INPUTS];  // 权重
    double bias;  // 偏置
} Perceptron;

// 激活函数（阶跃函数）
int step_function(double x) {
    return (x >= 0) ? 1 : 0;
}

// 初始化感知机
void initialize_perceptron(Perceptron* p) {
    // 随机初始化权重和偏置
    for (int i = 0; i < NUM_INPUTS; i++) {
        p->weights[i] = (rand() % 2000 - 1000) / 1000.0;  // -1到1之间的随机值
    }
    p->bias = 0.0;
}

// 计算感知机输出
int predict(Perceptron* p, double inputs[NUM_INPUTS]) {
    double sum = p->bias;
    for (int i = 0; i < NUM_INPUTS; i++) {
        sum += p->weights[i] * inputs[i];
    }
    return step_function(sum);
}

// 训练感知机
void train(Perceptron* p, double training_inputs[][NUM_INPUTS], int* labels, int num_samples) {
    for (int epoch = 0; epoch < MAX_ITER; epoch++) {
        int error_count = 0;
        for (int i = 0; i < num_samples; i++) {
            int prediction = predict(p, training_inputs[i]);
            int error = labels[i] - prediction;
            if (error != 0) {
                // 更新权重和偏置
                for (int j = 0; j < NUM_INPUTS; j++) {
                    p->weights[j] += LEARNING_RATE * error * training_inputs[i][j];
                }
                p->bias += LEARNING_RATE * error;
                error_count++;
            }
        }
        // 如果没有错误，说明训练完成
        if (error_count == 0) {
            break;
        }
    }
}

// 测试模型
void test(Perceptron* p, double test_inputs[][NUM_INPUTS], int* test_labels, int num_samples) {
    for (int i = 0; i < num_samples; i++) {
        int prediction = predict(p, test_inputs[i]);
        printf("Test sample %d: Predicted = %d, Actual = %d\n", i + 1, prediction, test_labels[i]);
    }
}

int main() {
    srand(time(NULL));

    // 定义训练数据（例如逻辑与运算 AND）
    double training_inputs[4][NUM_INPUTS] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };
    int labels[4] = { 0, 0, 0, 1 };  // AND 运算的标签

    // 初始化感知机
    Perceptron p;
    initialize_perceptron(&p);

    // 训练感知机
    train(&p, training_inputs, labels, 4);

    // 测试模型
    test(&p, training_inputs, labels, 4);

    return 0;
}