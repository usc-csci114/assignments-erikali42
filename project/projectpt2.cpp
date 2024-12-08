#include <iostream>
#include <vector>
#include <fstream>
#include <dlib/svm.h>
#include <string>
#include <sstream>

using namespace std;
using namespace dlib;

// Convert 'Yes'/'No' to 1/0
int yesNoToInt(const string& s) {
    if(s == "Yes"){
        return 1;
    }
    else{
        return 0;
    }
}

int main(int argc, char* argv[])
{
    typedef dlib::matrix<double, 7, 1> sample_type; // Dlib matrix that holds a feature vector for each sample

    std::vector<sample_type> samples; // Vector of feature vectors
    std::vector<double> labels; // Vector of labels of whether an individual has hair loss (0/1)

    ifstream ifile("hair_health.csv");
    string temp;

    // Read the header
    getline(ifile, temp);

    typedef linear_kernel<sample_type> kernel_type;

    size_t line_number = 0;
    while (getline(ifile, temp)) {
        string id, genetics, hormonalChanges, medicalConditions, medications, nutritionalDeficiencies, stress, age, hairCare, environmentalFactors, smoking, weightLoss, hairLoss;

        stringstream ss(temp);
        getline(ss, id, ',');
        getline(ss, genetics, ',');
        getline(ss, hormonalChanges, ',');
        getline(ss, medicalConditions, ',');
        getline(ss, medications, ',');
        getline(ss, nutritionalDeficiencies, ',');
        getline(ss, stress, ',');
        getline(ss, age, ',');
        getline(ss, hairCare, ',');
        getline(ss, environmentalFactors, ',');
        getline(ss, smoking, ',');
        getline(ss, weightLoss, ',');
        getline(ss, hairLoss);

        // Keeping only binary variables
        sample_type sample;
        sample(1) = yesNoToInt(genetics);
        sample(2) = yesNoToInt(hormonalChanges);
        sample(3) = yesNoToInt(environmentalFactors);
        sample(4) = yesNoToInt(smoking);
        sample(5) = yesNoToInt(hairCare);
        sample(6) = yesNoToInt(weightLoss);

        double label;
        int hairLossInt = std::stoi(hairLoss);
        if (hairLossInt == 1) {
            label = 1.0;
        } 
        else {
            label = 0.0;
        }        
        samples.push_back(sample);
        labels.push_back(label);
    }

    // Train SVM
    svm_c_trainer<kernel_type> trainer;
    decision_function<kernel_type> dec_funct = trainer.train(samples, labels);

    // Use the trained model to predict
    std::vector<double> predictions;
    int tp = 0, tn = 0, fp = 0, fn = 0;

    // Loop over the samples to predict and update confusion matrix
    for (size_t i = 0; i < samples.size(); ++i) {
        double prediction = dec_funct(samples[i]);
        predictions.push_back(prediction);

        // Compare predicted label to actual label to update the confusion matrix
        if (prediction == 1.0 && labels[i] == 1.0) {
            tp++;  // True Positive
        } else if (prediction == 0.0 && labels[i] == 0.0) {
            tn++;  // True Negative
        } else if (prediction == 1.0 && labels[i] == 0.0) {
            fp++;  // False Positive
        } else if (prediction == 0.0 && labels[i] == 1.0) {
            fn++;  // False Negative
        }
    }

    // Print the confusion matrix
    cout << "Confusion Matrix: " << endl;
    cout << "True Positive (TP): " << tp << endl;
    cout << "True Negative (TN): " << tn << endl;
    cout << "False Positive (FP): " << fp << endl;
    cout << "False Negative (FN): " << fn << endl;

    double accuracy = double(tp + tn) / (tp + tn + fp + fn);
    cout << "Accuracy: " << accuracy << endl;

    return 0;
}
