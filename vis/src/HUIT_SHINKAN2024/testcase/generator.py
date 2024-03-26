import random

def shuffle_2d_array(n):
    # 0からn*n-1までの整数のリストを生成してシャッフルする
    numbers = list(range(n * n))
    random.shuffle(numbers)
    
    # 二次元配列を生成する
    array_2d = [[0] * n for _ in range(n)]
    
    # シャッフルされた整数リストから二次元配列を作成する
    for i in range(n):
        for j in range(n):
            array_2d[i][j] = numbers[i * n + j]
    
    return array_2d

n = int(input())

shuffled_array = shuffle_2d_array(n)
for row in shuffled_array:
    print(*row, sep=' ')