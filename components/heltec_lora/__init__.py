
from esphome import automation
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

heltec_ns = cg.esphome_ns.namespace("heltec_lora")
HeltecLoRa = heltec_ns.class_("HeltecLoRaComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema({cv.GenerateID(): cv.declare_id(HeltecLoRa)})

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
