/*program from Wolfycz*/
#include<vector>
#include<cstring>
#include<iostream>
using namespace std;
class Matrix {
private:
    int n, m;
    vector<vector<int>> mat;
public:
    Matrix(int n = 2, int m = 2):n(n), m(m) {  //n:row, m:column
        for (int i = 0; i < n; i++) {
            mat.push_back(vector<int>(m, 0));
        }
        vector<vector<int>>(mat).swap(mat); //Remove excess capacity
    }
    Matrix(Matrix&& ots):n(ots.n), m(ots.m), mat(ots.mat) {}    //move construction
    Matrix(const Matrix& ots):n(ots.n), m(ots.m), mat(ots.mat) {}   //copy construction
    Matrix(const vector<vector<int>>& mat):mat(mat) { //construc from vector(Binary)
        vector<vector<int>>(this->mat).swap(this->mat);
        n = this->mat.size(), m = !n ? 0 : this->mat.back().size();
    }
    Matrix& operator=(const Matrix& ots) {
        if (&ots == this) return *this;
        mat = ots.mat;
        n = ots.n, m = ots.m;
        return *this;
    }
    Matrix& operator=(Matrix&& ots) {
        if (&ots == this) return *this;
        mat = ots.mat;
        n = ots.n, m = ots.m;
        return *this;
    }
    Matrix& operator=(const vector<vector<int>>& mat) {   //Assign with vector(Binary)
        this->mat = mat;
        vector<vector<int>>(this->mat).swap(this->mat);
        n = this->mat.size(), m = !n ? 0 : this->mat.back().size();
        return *this;
    }
    void reshape(int n, int m) {    //surplus cut, deficiency fill zero
        vector<vector<int>>newValue;
        for (int i = 0; i < n; i++)
            newValue.push_back(vector<int>(m, 0));
        int Cnt = -1;
        for (int i = 0; i < this->n; i++) {
            for (int j = 0; j < this->m; j++) {
                if (++Cnt == n * m)
                    goto reshapeOut;
                newValue[Cnt / m][Cnt % m] = mat[i][j];
            }

        }
    reshapeOut:
        this->n = n, this->m = m;
        mat = newValue;
        vector<vector<int>>(mat).swap(mat);
    }
    void set(int v) {   //fill with v
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                mat[i][j] = v;
    }
    void reset() { set(0); }
    void clear() { vector<vector<int>>().swap(mat); } //Remove all capacity
    void Identity() {  // Identity matrix
        reset();
        try {
            if (n != m) {
                printf("error throw in I()\n");
                throw   string("Matrix is not square");
            }
            for (int i = 0; i < n; i++)
                mat[i][i] = 1;
        } catch (string info) {
            cout << info << endl;
            exit(0);
        }
    }
    int& at(int x, int y) { return mat[x][y]; }   //Another way to use subscripts
    void print() {  //Commonly used in debug
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++)
                printf("%d ", mat[i][j]);
            putchar('\n');
        }
    }
    int getn() const { return n; }
    int getm() const { return m; }
    friend Matrix operator+(const Matrix& A, const Matrix& B);
    friend Matrix operator-(const Matrix& A, const Matrix& B);
    friend Matrix operator*(const Matrix& A, const Matrix& B);  //multiplication cross
};
Matrix operator+(const Matrix& A, const Matrix& B) {
    try {
        if (A.n != B.n || A.m != B.m) {
            printf("error throw in +(Matrix)\n");
            throw string("Shape of the matrixes must be the same");
        }
        Matrix C = A;
        for (int i = 0; i < C.n; i++)
            for (int j = 0; j < C.m; j++)
                C.mat[i][j] += B.mat[i][j];
        return C;
    } catch (string info) {
        cout << info << endl;
        exit(0);
    }
    return Matrix();
}
Matrix operator-(const Matrix& A, const Matrix& B) {
    try {
        if (A.n != B.n || A.m != B.m) {
            printf("error throw in -(Matrix)\n");
            throw string("Shape of the matrixes must be the same");
        }
        Matrix C = A;
        for (int i = 0; i < C.n; i++)
            for (int j = 0; j < C.m; j++)
                C.mat[i][j] -= B.mat[i][j];
        return C;
    } catch (string info) {
        cout << info << endl;
        exit(0);
    }
    return Matrix();
}
Matrix operator*(const Matrix& A, const Matrix& B) {
    try {
        if (A.m != B.n) {
            printf("error throw in *(Matrix)\n");
            throw string("The columns of the first must be equal to the rows of the second");
        }
        Matrix C;
        for (int i = 0; i < A.n; i++)
            for (int k = 0; k < A.m; k++)
                for (int j = 0; j < B.m; j++)
                    C.mat[i][j] += A.mat[i][k] * B.mat[k][j];
        C.n = A.n, C.m = B.m;
        return C;
    } catch (string info) {
        cout << info << endl;
        exit(0);
    }
    return Matrix();
}
int main() {
    return 0;
}