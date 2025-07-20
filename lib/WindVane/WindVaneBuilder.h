#pragma once
#include "WindVane.h"
#include <memory>

// Builder pattern to simplify WindVane construction
class WindVaneBuilder {
private:
    IADC* _adc = nullptr;
    WindVaneType _type = WindVaneType::REED_SWITCH;
    CalibrationMethod _method = CalibrationMethod::SPINNING;
    ICalibrationStorage* _storage = nullptr;
    IUserIO* _io = nullptr;
    IDiagnostics* _diag = nullptr;
    CalibrationConfig _config{};

public:
    WindVaneBuilder& withADC(IADC& adc) {
        _adc = &adc;
        return *this;
    }
    
    WindVaneBuilder& withType(WindVaneType type) {
        _type = type;
        return *this;
    }
    
    WindVaneBuilder& withCalibrationMethod(CalibrationMethod method) {
        _method = method;
        return *this;
    }
    
    WindVaneBuilder& withStorage(ICalibrationStorage& storage) {
        _storage = &storage;
        return *this;
    }
    
    WindVaneBuilder& withIO(IUserIO& io) {
        _io = &io;
        return *this;
    }
    
    WindVaneBuilder& withDiagnostics(IDiagnostics& diag) {
        _diag = &diag;
        return *this;
    }
    
    WindVaneBuilder& withConfig(const CalibrationConfig& config) {
        _config = config;
        return *this;
    }
    
    std::unique_ptr<WindVane> build() {
        if (!_adc || !_io || !_diag) {
            throw std::invalid_argument("Required dependencies not provided");
        }
        
        WindVaneConfig cfg{
            *_adc, _type, _method, _storage, *_io, *_diag, _config
        };
        return std::make_unique<WindVane>(cfg);
    }
};