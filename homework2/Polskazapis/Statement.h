//
// Created by artem on 04.10.2024.
//

#ifndef CPP_SEMINARS_STATEMENT_H
#define CPP_SEMINARS_STATEMENT_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <sstream>

class Statement {
public:
    virtual std::vector<int> apply(std::vector<int> in) const = 0;

    Statement() = default;
    Statement(unsigned arguments, unsigned results, bool pure): arguments(arguments), results(results), pure(pure) {}

    virtual ~Statement() = default;

    bool is_pure() const {
        return pure;
    }

    unsigned get_arguments_count() const {
        return arguments;
    }

    unsigned get_results_count() const {
        return results;
    }

protected:
    unsigned arguments;
    unsigned results;
    bool pure;
};

class Combine : public Statement {
public:
    Combine() = default;
    Combine(const Combine &c) = default;
    Combine(std::shared_ptr<Statement> l, std::shared_ptr<Statement> r): Statement(), l(l), r(r) {
        arguments = l->get_arguments_count() + r->get_arguments_count() - std::min(l->get_results_count(), r->get_arguments_count());
        results = l->get_results_count() + r->get_results_count() - std::min(l->get_results_count(), r->get_arguments_count());
        pure = l->is_pure() & r->is_pure();
    }
    std::vector<int> apply(std::vector<int> in) const override {
        return std::move(r->apply(std::move(l->apply(std::move(in)))));
    }
private:
    std::shared_ptr<Statement> l;
    std::shared_ptr<Statement> r;
};

class ConstOp : public Statement {

public:
    ConstOp(int v): Statement(0, 1, true), v(v) {}

    std::vector<int> apply(std::vector<int> in) const override {
        in.push_back(v);
        return in;
    }

private:
    int v;
};

class PlusOp: public Statement {
public:
    PlusOp(): Statement(2, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int secondOperand = in.back();
        in.pop_back();
        int firstOperand = in.back();
        in.pop_back();
        in.push_back(firstOperand + secondOperand);
        return in;
    }
};

class MinusOp: public Statement {
public:
    MinusOp(): Statement(2, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int secondOperand = in.back();
        in.pop_back();
        int firstOperand = in.back();
        in.pop_back();
        in.push_back(firstOperand - secondOperand);
        return in;
    }
};

class MultOp: public Statement {
public:
    MultOp(): Statement(2, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int secondOperand = in.back();
        in.pop_back();
        int firstOperand = in.back();
        in.pop_back();
        in.push_back(firstOperand * secondOperand);
        return in;
    }
};

class DivOp: public Statement {
public:
    DivOp(): Statement(2, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int secondOperand = in.back();
        in.pop_back();
        int firstOperand = in.back();
        in.pop_back();
        in.push_back(firstOperand / secondOperand);
        return in;
    }
};

class ModOp: public Statement {
public:
    ModOp(): Statement(2, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int secondOperand = in.back();
        in.pop_back();
        int firstOperand = in.back();
        in.pop_back();
        in.push_back(firstOperand % secondOperand);
        return in;
    }
};

class AbsOp: public Statement {
public:
    AbsOp(): Statement(1, 1, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int firstOperand = abs(in.back());
        in.pop_back();
        in.push_back(firstOperand);
        return in;
    }
};

class InputOp: public Statement {
public:
    InputOp(): Statement(0, 1, false) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int firstOperand;
        std::cin >> firstOperand;
        in.push_back(firstOperand);
        return in;
    }
};

class DupOp: public Statement {
public:
    DupOp(): Statement(1, 2, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        int firstOperand = in.back();
        in.pop_back();
        in.push_back(firstOperand);
        in.push_back(firstOperand);
        return in;
    }
};

class Nop: public Statement {
public:
    Nop(): Statement(0, 0, true) {}

    std::vector<int> apply(std::vector<int> in) const override {
        return in;
    }
};

std::string getWord(std::string_view& str, int& idx) {
    std::string ans = "";
    while (idx < (int)str.size() && str[idx] == ' ') {
        ++idx;
    }
    while (idx < (int)str.size()) {
        if (str[idx] == ' ') {
            return ans;
        }
        ans += str[idx];
        ++idx;
    }
    return ans;
}

std::shared_ptr<Statement> compile(std::string_view str) {
    std::string word;
    std::vector<int> in;
    std::shared_ptr<Statement> prevOp = std::make_shared<Nop>();
    std::shared_ptr<Statement> curOp;
    int idx = 0;
    word = getWord(str, idx);
    while (word != "") {
        if (isdigit(word[0])) {
            ConstOp op(std::stoi(word));
            curOp = std::make_shared<ConstOp>(op);
        } else if (word[0] == '+' && word.size() > 1) {
            ConstOp op(std::stoi(word.substr(1, word.size())));
            curOp = std::make_shared<ConstOp>(op);
        } else if (word[0] == '-' && word.size() > 1) {
            ConstOp op(std::stoi(word.substr(1, word.size())) * -1);
            curOp = std::make_shared<ConstOp>(op);
        } else if (word[0] == '+') {
                PlusOp op;
                curOp = std::make_shared<PlusOp>(op);
        } else if (word[0] == '-') {
            MinusOp op;
            curOp = std::make_shared<MinusOp>(op);
        } else if (word[0] == '*') {
            MultOp op;
            curOp = std::make_shared<MultOp>(op);
        } else if (word[0] == '/') {
            DivOp op;
            curOp = std::make_shared<DivOp>(op);
        } else if (word[0] == '%') {
            ModOp op;
            curOp = std::make_shared<ModOp>(op);
        } else if (word[0] == 'a') {
            AbsOp op;
            curOp = std::make_shared<AbsOp>(op);
        } else if (word[0] == 'i') {
            InputOp op;
            curOp = std::make_shared<InputOp>(op);
        } else if (word[0] == 'd') {
            DupOp op;
            curOp = std::make_shared<DupOp>(op);
        } else {
            Nop op;
            curOp = std::make_shared<Nop>(op);
        }
        prevOp = std::make_shared<Combine>(Combine(prevOp, curOp));
        word = getWord(str, idx);
    }
    return prevOp;
}


std::shared_ptr<Statement> operator|(std::shared_ptr<Statement> lhs, std::shared_ptr<Statement> rhs) {
    return std::make_shared<Combine>(Combine(lhs, rhs));
}


#endif //CPP_SEMINARS_STATEMENT_H
