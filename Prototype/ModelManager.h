#ifndef _MODEL_MANAGER_H_ 
#define _MODEL_MANAGER_H_

#include <unordered_map>
#include <string>
#include "Model.h"

class ModelManager
{
private:
	std::unordered_map<std::string, Model> m_mModels;

	Model m_defModel;
public:
	 ModelManager();
	~ModelManager();

	const Model& getModel(const std::string& id) const;
};

#endif
