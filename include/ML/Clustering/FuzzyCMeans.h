#pragma once
#include <random>
#include <vector>
using Vector = std::vector<double>;
using Matrix = std::vector<std::vector<double>>;
class FuzzyCMeans {
private:
	int numClusters;
	double fuzziness;		//deg to which a point is allowed to belong to multiple clusters
	int maxIts;				//alg stops when converges ekse maxIts is safety limit
	double tolerance;		//iterations stop when diff<tolerance
	Matrix centers;			//centers[clusterNum][feature]
	Matrix memberships;		//memberships[datapts][clusters]
	unsigned int randomSeed;			//for reproducibility
	double calculateDist(const Vector& a, const Vector& b) const;		//helper function
	void initialiseMemberships(int numDatapts);
	void updateCenters(const Matrix &data);
	void updateMemberships(const Matrix& data);
	bool hasConverged(Matrix &oldMemberships);
	bool trained = false;
	double calcMaxDifference(const Matrix& oldMemberships)const;
	

public:
	struct TrainingInfo {
		int iterations;
		bool hasConverged;
		double finalError;
	};
	FuzzyCMeans(int clusters, double fuzziness = 2.0, int maxIts = 100, double tolerance = 0.001, unsigned int seed =std::random_device{}());
	void fit(const Matrix& data);
	Vector predict(const Vector& point) const;
	int predictCluster(const Vector& point) const;
	Matrix getCenters() const;
	Matrix getMemberships() const;
	TrainingInfo getTrainingInfo() const;
private:
	TrainingInfo traininginfo;



};