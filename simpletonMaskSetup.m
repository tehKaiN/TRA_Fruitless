set_param(gcb,'Mask','on')
set_param(gcb,'Name','simpleton')
set_param(gcb,'BackgroundColor','[0.721569, 0.858824, 0.905882]')
set_param(gcb,'MaskDisplay','port_label(''input'',1,''Input_Interface_Component.''); port_label(''output'',1,''Output_Interface_Component.''); port_label(''output'',2,''DEBUG'')');
set_param(gcb,'MaskPrompts',{})
set_param(gcb,'MaskVariables','')
defaultVals = {};
vals = get_param(gcb,'MaskValues')';
for i=1:length(vals)
    if strcmp(vals{i}, '[]')
        vals{i} = '';
    end
    if isempty(vals{i})
        vals{i} = defaultVals{i};
    end
end
set_param(gcb,'MaskValues', vals)
