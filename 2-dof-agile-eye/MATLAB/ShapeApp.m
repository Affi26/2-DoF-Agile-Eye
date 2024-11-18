

classdef ShapeApp < matlab.apps.AppBase

    % Properties that correspond to app components
    properties (Access = public)

        UIFigure                matlab.ui.Figure
        TabGroup                matlab.ui.container.TabGroup
        
        ShapeTab                matlab.ui.container.Tab
        ParametersTab           matlab.ui.container.Tab
        SerialTab               matlab.ui.container.Tab

        SquareButton            matlab.ui.control.Button
        TriangleButton          matlab.ui.control.Button
        CircleButton            matlab.ui.control.Button
        ExecuteButton           matlab.ui.control.Button
        CreateButton            matlab.ui.control.Button
        OpenPortButton          matlab.ui.control.Button
        ClosePortButton         matlab.ui.control.Button
        SendButton              matlab.ui.control.Button

        UIAxesShape             matlab.ui.control.UIAxes
        UIAxesParameters        matlab.ui.control.UIAxes
        UIAxesSerial            matlab.ui.control.UIAxes

        SideLengthEditField     matlab.ui.control.NumericEditField
        BaseLengthEditField     matlab.ui.control.NumericEditField
        HeightEditField         matlab.ui.control.NumericEditField
        RadiusEditField         matlab.ui.control.NumericEditField
        WallDistanceEditField   matlab.ui.control.NumericEditField
        LoopCountEditField      matlab.ui.control.NumericEditField
        IntervalEditField       matlab.ui.control.NumericEditField
        SetPointsEditField      matlab.ui.control.NumericEditField
        XOffsetEditField        matlab.ui.control.NumericEditField
        YOffsetEditField        matlab.ui.control.NumericEditField
        MaxSpacingEditField     matlab.ui.control.NumericEditField

        SquareSideLengthLabel   matlab.ui.control.Label
        TriangleBaseLengthLabel matlab.ui.control.Label
        TriangleHeightLabel     matlab.ui.control.Label
        CircleRadiusLabel       matlab.ui.control.Label
        WallDistanceLabel       matlab.ui.control.Label
        LoopCountLabel          matlab.ui.control.Label
        IntervalLabel           matlab.ui.control.Label
        SetPointsLabel          matlab.ui.control.Label
        XOffsetLabel            matlab.ui.control.Label
        YOffsetLabel            matlab.ui.control.Label
        MaxSpacingLabel         matlab.ui.control.Label

        MessageTextArea         matlab.ui.control.TextArea

        SerialPort

        Mode                    char % 'coordinates' or 'angles' mode
        ModeSwitch              matlab.ui.control.Switch
        
        SerialPortDropDown      matlab.ui.control.DropDown

        SelectedShape           char
        ShapeCoordinates
        AngleCoordinates
        zCoordinate
        MaxSpacing
        LoopCount
        closed
        Execute
        SerialMsg               char
    end

    % Callbacks that handle component events
    methods (Access = public)

        % Code that executes after component creation
        function startupFcn(app)
            % Initialize UIAxes
            disp('Starting application...');
            pause(0.5);
            cla(app.UIAxesShape);
            cla(app.UIAxesParameters);
            cla(app.UIAxesSerial);
            app.closed = false;
            app.Execute = 0;
        end

        % Button pushed function: SquareButton
        function squareButtonPushed(app, ~)

            app.SelectedShape = 'square';

            % Enable square-related fields
            app.SideLengthEditField.Visible = 'on';
            app.SquareSideLengthLabel.Visible = 'on';
            
            % Disable triangle-related fields
            app.BaseLengthEditField.Visible = 'off';
            app.HeightEditField.Visible = 'off';
            app.TriangleBaseLengthLabel.Visible = 'off';
            app.TriangleHeightLabel.Visible = 'off';

            % Disable circle-related fields
            app.RadiusEditField.Visible = 'off';
            app.SetPointsEditField.Visible = 'off';
            app.CircleRadiusLabel.Visible = 'off';
            app.SetPointsLabel.Visible = 'off';

        end

        % Button pushed function: TriangleButton
        function triangleButtonPushed(app, ~)

            app.SelectedShape = 'triangle';

            % Enable triangle-related fields
            app.HeightEditField.Visible = 'on';
            app.BaseLengthEditField.Visible = 'on';
            app.TriangleHeightLabel.Visible = 'on';
            app.TriangleBaseLengthLabel.Visible = 'on';
            
            % Disable square-related fields
            app.SideLengthEditField.Visible = 'off';
            app.SquareSideLengthLabel.Visible = 'off';

            % Disable circle-related fields
            app.RadiusEditField.Visible = 'off';
            app.SetPointsEditField.Visible = 'off';
            app.CircleRadiusLabel.Visible = 'off';
            app.SetPointsLabel.Visible = 'off';

        end

        % Define callback function for the circle button
        function circleButtonPushed(app, ~)

            app.SelectedShape = 'circle';

            % Enable circle-related fields
            app.RadiusEditField.Visible = 'on';
            app.SetPointsEditField.Visible = 'on';
            app.CircleRadiusLabel.Visible = 'on';
            app.SetPointsLabel.Visible = 'on';
            
            % Disable square and triangle-related fields
            app.SideLengthEditField.Visible = 'off';
            app.SquareSideLengthLabel.Visible = 'off';

            app.BaseLengthEditField.Visible = 'off';
            app.HeightEditField.Visible = 'off';
            app.TriangleBaseLengthLabel.Visible = 'off';
            app.TriangleHeightLabel.Visible = 'off';
            

        end

        % Button pushed function: CreateButton
        function createButtonPushed(app, ~)

            % Clear previous shape
            cla(app.UIAxesShape);
            cla(app.UIAxesParameters);
            cla(app.UIAxesSerial);


            % Store values from distance to wall and loop count fields
            app.zCoordinate = app.WallDistanceEditField.Value;
            app.LoopCount = app.LoopCountEditField.Value;
            app.MaxSpacing = app.MaxSpacingEditField.Value;
               
            % Store offset values
            xOffset = app.XOffsetEditField.Value;
            yOffset = app.YOffsetEditField.Value;
            
            if strcmp(app.SelectedShape, 'square')

                % Draw square

                sideLength = app.SideLengthEditField.Value;

                % Adjust position so center is at (0,0)
                xCenter = -sideLength/2 + xOffset;
                yCenter = -sideLength/2 + yOffset;
                rectangle(app.UIAxesShape, 'Position', [xCenter, yCenter, sideLength, sideLength], 'EdgeColor', 'r');
                rectangle(app.UIAxesParameters, 'Position', [xCenter, yCenter, sideLength, sideLength], 'EdgeColor', 'r');
                rectangle(app.UIAxesSerial, 'Position', [xCenter, yCenter, sideLength, sideLength], 'EdgeColor', 'r');

                % Adjust axes limits
                axis(app.UIAxesShape, 'equal');
                xlim(app.UIAxesShape, [xCenter - 0.5 * sideLength, xCenter + 1.5 * sideLength]);
                ylim(app.UIAxesShape, [yCenter - 0.5 * sideLength, yCenter + 1.5 * sideLength]);

                axis(app.UIAxesParameters, 'equal');
                xlim(app.UIAxesParameters, [xCenter - 0.5 * sideLength, xCenter + 1.5 * sideLength]);
                ylim(app.UIAxesParameters, [yCenter - 0.5 * sideLength, yCenter + 1.5 * sideLength]);

                axis(app.UIAxesSerial, 'equal');
                xlim(app.UIAxesSerial, [xCenter - 0.5 * sideLength, xCenter + 1.5 * sideLength]);
                ylim(app.UIAxesSerial, [yCenter - 0.5 * sideLength, yCenter + 1.5 * sideLength]);
                
                % Get square coordinates
                square_pos = app.UIAxesShape.Children(1).Position;
                square_corners = [square_pos(1), square_pos(2); % bottom-left corner
                                  square_pos(1) + square_pos(3), square_pos(2); % bottom-right corner
                                  square_pos(1) + square_pos(3), square_pos(2) + square_pos(4); % top-right corner
                                  square_pos(1), square_pos(2) + square_pos(4); % top-left corner
                                  square_pos(1), square_pos(2)]; % bottom-left corner again
                


                % Round coordinates smaller than abs(1e-5) to 0
                square_corners(abs(square_corners) <= 1e-5) = 0;

                % Invert y-coordinates here (for better pointer axes
                % representation)
                %square_corners(:,2) = -square_corners(:,2);

                % Assign square coordinates to a property of the app class
                app.ShapeCoordinates = square_corners;

            elseif strcmp(app.SelectedShape, 'triangle')

                % Draw triangle
                baseLength = app.BaseLengthEditField.Value;
                height = app.HeightEditField.Value;

                % Set center at (0,0)
                x = [xOffset - baseLength/2, xOffset + baseLength/2, xOffset, xOffset - baseLength/2];
                y = [yOffset - height/2, yOffset - height/2, yOffset + height/2, yOffset - height/2]; % Invert y-offset
                plot(app.UIAxesShape, x, y, 'r');
                plot(app.UIAxesParameters, x, y, 'r');
                plot(app.UIAxesSerial, x, y, 'r');

                % Adjust axes limits
                axis(app.UIAxesShape, 'equal');
                xlim(app.UIAxesShape, [min(x) - 0.5 * baseLength, max(x) + 0.5 * baseLength]);
                ylim(app.UIAxesShape, [min(y) - 0.5 * height, max(y) + 0.5 * height]);
                
                axis(app.UIAxesParameters, 'equal');
                xlim(app.UIAxesParameters, [min(x) - 0.5 * baseLength, max(x) + 0.5 * baseLength]);
                ylim(app.UIAxesParameters, [min(y) - 0.5 * height, max(y) + 0.5 * height]);

                axis(app.UIAxesSerial, 'equal');
                xlim(app.UIAxesSerial, [min(x) - 0.5 * baseLength, max(x) + 0.5 * baseLength]);
                ylim(app.UIAxesSerial, [min(y) - 0.5 * height, max(y) + 0.5 * height]);
                
                % Get triangle coordinates
                triangle = get(app.UIAxesShape.Children(1), {'XData', 'YData'});
                triangle_corners = [triangle{1}(1), triangle{2}(1);   % first vertex
                                    triangle{1}(2), triangle{2}(2);   % second vertex
                                    triangle{1}(3), triangle{2}(3);   % third vertex
                                    triangle{1}(1), triangle{2}(1)];  % closing vertex
                
                % Round coordinates less than or equal to 1e-5 to 0
                triangle_corners(abs(triangle_corners) <= 1e-5) = 0;

                % Invert y-coordinates for better pointer direction
                %triangle_corners(:,2) = -triangle_corners(:,2);

                % Assign triangle coordinates to a property of the app class
                app.ShapeCoordinates = triangle_corners;

            elseif strcmp(app.SelectedShape, 'circle')

                % Draw circle
                radius = app.RadiusEditField.Value;
                setPoints = app.SetPointsEditField.Value;
                
                % Generate circle coordinates
                x_center = xOffset;
                y_center = yOffset;
                theta = linspace(0, 2*pi, setPoints); % Adjust the number of points based on user input

                x_circle = x_center + radius * cos(theta);
                y_circle = y_center + radius * sin(theta);

                circle_coordinates = [x_circle', y_circle'];
                plot(app.UIAxesShape, x_circle, y_circle, 'r');
                plot(app.UIAxesParameters, x_circle, y_circle, 'r');
                plot(app.UIAxesSerial, x_circle, y_circle, 'r');

                % Adjust axes limits
                axis(app.UIAxesShape, 'equal');
                xlim(app.UIAxesShape, [x_center - 1.5 * radius, x_center + 1.5 * radius]);
                ylim(app.UIAxesShape, [y_center - 1.5 * radius, y_center + 1.5 * radius]);
                
                axis(app.UIAxesParameters, 'equal');
                xlim(app.UIAxesParameters, [x_center - 1.5 * radius, x_center + 1.5 * radius]);
                ylim(app.UIAxesParameters, [y_center - 1.5 * radius, y_center + 1.5 * radius]);

                axis(app.UIAxesSerial, 'equal');
                xlim(app.UIAxesSerial, [x_center - 1.5 * radius, x_center + 1.5 * radius]);
                ylim(app.UIAxesSerial, [y_center - 1.5 * radius, y_center + 1.5 * radius]);

                % Round coordinates less than or equal to 1e-5 to 0
                circle_coordinates(abs(circle_coordinates) <= 1e-5) = 0;

                % Invert y-coordinates for better pointer representation
                %circle_coordinates(:,2) = -circle_coordinates(:,2);

                % Assign circle coordinates to a property of the app class
                app.ShapeCoordinates = circle_coordinates;
            end

            % Generate angle data as well
            app.AngleCoordinates = app.convertCoordinatesToAngles(app.ShapeCoordinates);

            % Rotate coordinate data 90 deg ccw
            rotation_matrix = [0 -1; 1 0];
            app.ShapeCoordinates = (rotation_matrix * app.ShapeCoordinates')';
        
        end

        % Method to convert coordinates to angles
        function angles = convertCoordinatesToAngles(app, coordinates)
            angles = zeros(size(coordinates, 1), 2);
            for i = 1:size(coordinates, 1)
                x_coord = coordinates(i, 1);
                y_coord = -coordinates(i, 2); % Invert y-coordinates here
                z_coord = app.zCoordinate;
                angle1 = atan2(-y_coord, z_coord) * (180/pi);
                angle2 = atan2(-x_coord * z_coord, (y_coord^2) + (z_coord^2)) * (180/pi);
                angles(i, :) = [angle1, angle2];
            end
        end

        % Button pushed function: ExecuteButton for sending serial message
        function executeButtonPushed(app, ~)
            
            % Generate serial message
            app.MessageTextArea.Value = 'Creating serial message...';
            app.SerialMsg = app.compileSerialMsg();

            pause(1);

            if ~isempty(app.SerialMsg)
                app.MessageTextArea.Value = sprintf('Serial message ready: %s\n', app.SerialMsg);
            end

            pause(1);

        end

        % Function to compile serial message
        function SerialMsg = compileSerialMsg(app)

            % Initialize an empty character array to store 
            % the serial message
            SerialMsg = '';

            % Determine the mode prefix
            if strcmp(app.Mode, 'coordinates')
                modePrefix = 'c';
                data = app.ShapeCoordinates; % Use coordinate data
            elseif strcmp(app.Mode, 'angles')
                modePrefix = 'a';
                data = app.AngleCoordinates; % Use angle data
            else
                % Handle invalid mode
                disp('Invalid mode.');
                return;
            end

            % Create start of serial message
            SerialMsg = [modePrefix, ';', num2str(size(app.ShapeCoordinates, 1)), ';'];

            % Append loop count if set, otherwise default to 0
            if ~isempty(app.LoopCount)
                SerialMsg = [SerialMsg, num2str(app.LoopCount)];
            else
                SerialMsg = [SerialMsg, '0'];
            end
                
            SerialMsg = [SerialMsg, ';'];

            % Append interval value
            SerialMsg = [SerialMsg, num2str(app.IntervalEditField.Value)];

            SerialMsg = [SerialMsg, ';'];

            % Use wall distance and max spacing only for coordinates mode
            if strcmp(app.Mode, 'coordinates')

                if ~isempty(app.zCoordinate)
                    SerialMsg = [SerialMsg, num2str(app.zCoordinate)];
                else
                    SerialMsg = [SerialMsg, '0'];
                end

                SerialMsg = [SerialMsg, ';'];

                if ~isempty(app.MaxSpacing)
                    SerialMsg = [SerialMsg, num2str(app.MaxSpacing)];
                else
                    SerialMsg = [SerialMsg, '0'];
                end

                SerialMsg = [SerialMsg, ';'];
            end


            % Loop through pairs of coordinate or angle data
            for i = 1:size(data, 1)
                % Append the x-coordinate and y-coordinate to the list
                SerialMsg = [SerialMsg, num2str(data(i, 1)), ',', num2str(data(i, 2)), ';'];
            end
                
            % Remove the trailing comma from the end of the list
            %coordinateList = coordinateList(1:end-1);
        end

        % Button pushed function: OpenPortButton
        function openPortButtonPushed(app, ~)
            port = app.SerialPortDropDown.Value;
            baudRate = 115200;
            % Check if the serial port is available
            if ismember(port, serialportlist("available"))
                try
                    % Create serial port object
                    app.MessageTextArea.Value = 'Initializing serial communication...';
                    pause(1);
                    app.SerialPort = serialport(port, baudRate);
                    app.MessageTextArea.Value = 'Port open';
                    
                    % Open the serial port
                    configureTerminator(app.SerialPort, "CR/LF");
                    flush(app.SerialPort);
                catch exception
                    % Handle port already in use error
                    app.MessageTextArea.Value = sprintf("Error: %s port is already in use.\n", port);
                    disp(exception.message);
                end
            else
                app.MessageTextArea.Value = sprintf("Serial port %s is not available.\n", port);
            end
        end
        
        % Button pushed function: ClosePortButton
        function closePortButtonPushed(app, ~)
            % Check if the serial port is not empty and is valid
            if ~isempty(app.SerialPort) && isvalid(app.SerialPort)
                % Check if the serial port is open
                if strcmp(app.SerialPort.Status, 'open')
                    % Close the serial port
                    app.MessageTextArea.Value = 'Closing serial port...';
                    delete(app.SerialPort);
                    app.MessageTextArea.Value = 'Port closed';
                else
                    app.MessageTextArea.Value = 'Serial port is not open.';
                end
            else
                app.MessageTextArea.Value = 'Serial port is not initialized.';
            end
        end

        % Method to send the serial message
        function sendSerialMessage(app, ~)

            % Send the serial message over the serial port
            if ~isempty(app.SerialPort) && strcmp(app.SerialPort.Status, 'open')
                app.MessageTextArea.Value = 'Sending serial message...';
                write(app.SerialPort, app.SerialMsg, 'char');
                pause(1);
                app.MessageTextArea.Value = 'Serial message sent';
            else
                app.MessageTextArea.Value = 'Serial port is not open.';
            end
        end

        % Callback function for ModeSwitch value changed event
        function modeSwitchValue(app, ~)
            % Get the selected mode from the switch
            mode = app.ModeSwitch.Value;
            
            % Update the Mode property accordingly
            if strcmp(mode, 'Coordinates')
                app.Mode = 'coordinates';
                %disp('coordinates');
            elseif strcmp(mode, 'Angles')
                app.Mode = 'angles';
                %disp('angles');
            end
        end



    end

    % App initialization and construction
    methods (Access = private)

        % Create UIFigure and components
        function createComponents(app)

            % Create UIFigure and configure properties
            app.UIFigure = uifigure;
            app.UIFigure.Position = [220 55 830 630];
            app.UIFigure.Name = 'ShapeApp';

            % Create a tab group
            app.TabGroup = uitabgroup(app.UIFigure);
            app.TabGroup.Position = [10 10 810 610]; % Adjust size and position as needed
        
            % Create tabs
            app.ShapeTab = uitab(app.TabGroup);
            app.ShapeTab.Title = 'Shape';
            
            app.ParametersTab = uitab(app.TabGroup);
            app.ParametersTab.Title = 'Parameters';
            
            app.SerialTab = uitab(app.TabGroup);
            app.SerialTab.Title = 'Serial';

            % Create UIAxes in the Shape tab
            app.UIAxesShape = uiaxes(app.ShapeTab); % Rename UIAxes to UIAxesShape
            title(app.UIAxesShape, 'Shape')
            xlabel(app.UIAxesShape, 'X')
            ylabel(app.UIAxesShape, 'Y')
            app.UIAxesShape.Position = [20 80 400 400]; % Adjust position and size as needed
            
            % Create UIAxes in the Parameters tab
            app.UIAxesParameters = uiaxes(app.ParametersTab); % Rename UIAxes to UIAxesParameters
            title(app.UIAxesParameters, 'Shape')
            xlabel(app.UIAxesParameters, 'X')
            ylabel(app.UIAxesParameters, 'Y')
            app.UIAxesParameters.Position = [20 80 400 400]; % Adjust position and size as needed
            
            % Create UIAxes in the Serial tab
            app.UIAxesSerial = uiaxes(app.SerialTab); % Rename UIAxes to UIAxesSerial
            title(app.UIAxesSerial, 'Shape')
            xlabel(app.UIAxesSerial, 'X')
            ylabel(app.UIAxesSerial, 'Y')
            app.UIAxesSerial.Position = [20 80 400 400]; % Adjust position and size as needed

        
            % Create buttons for selecting shapes in the Shape tab
            app.SquareButton = uibutton(app.ShapeTab, 'push');
            app.SquareButton.ButtonPushedFcn = createCallbackFcn(app, @squareButtonPushed, true);
            app.SquareButton.Position = [450 460 100 22];
            app.SquareButton.Text = 'Square';
            
            app.TriangleButton = uibutton(app.ShapeTab, 'push');
            app.TriangleButton.ButtonPushedFcn = createCallbackFcn(app, @triangleButtonPushed, true);
            app.TriangleButton.Position = [560 460 100 22];
            app.TriangleButton.Text = 'Triangle';
            
            app.CircleButton = uibutton(app.ShapeTab, 'push');
            app.CircleButton.ButtonPushedFcn = createCallbackFcn(app, @circleButtonPushed, true);
            app.CircleButton.Position = [670 460 100 22];
            app.CircleButton.Text = 'Circle';
        
            app.SquareSideLengthLabel = uilabel(app.ShapeTab);
            app.SquareSideLengthLabel.Text = 'Side Length (cm):';
            app.SquareSideLengthLabel.Position = [450 420 120 22];
            
            app.SideLengthEditField = uieditfield(app.ShapeTab, 'numeric');
            app.SideLengthEditField.Position = [560 420 100 22];
            app.SideLengthEditField.Value = 1;
            
            app.TriangleBaseLengthLabel = uilabel(app.ShapeTab);
            app.TriangleBaseLengthLabel.Text = 'Base Length (cm):';
            app.TriangleBaseLengthLabel.Position = [450 420 120 22];
            
            app.BaseLengthEditField = uieditfield(app.ShapeTab, 'numeric');
            app.BaseLengthEditField.Position = [560 420 100 22];
            app.BaseLengthEditField.Value = 1;
            
            app.TriangleHeightLabel = uilabel(app.ShapeTab);
            app.TriangleHeightLabel.Text = 'Height (cm):';
            app.TriangleHeightLabel.Position = [450 380 120 22];
            
            app.HeightEditField = uieditfield(app.ShapeTab, 'numeric');
            app.HeightEditField.Position = [560 380 100 22];
            app.HeightEditField.Value = 1;
            
            app.CircleRadiusLabel = uilabel(app.ShapeTab);
            app.CircleRadiusLabel.Text = 'Radius (cm):';
            app.CircleRadiusLabel.Position = [450 420 120 22];
            
            app.RadiusEditField = uieditfield(app.ShapeTab, 'numeric');
            app.RadiusEditField.Position = [560 420 100 22];
            app.RadiusEditField.Value = 1;
            
            app.SetPointsLabel = uilabel(app.ShapeTab);
            app.SetPointsLabel.Text = 'Set Points:';
            app.SetPointsLabel.Position = [450 380 120 22];
            
            app.SetPointsEditField = uieditfield(app.ShapeTab, 'numeric');
            app.SetPointsEditField.Position = [560 380 100 22];
            app.SetPointsEditField.Value = 10; % Default value
        
            % Create labels and edit fields for other parameters in the Parameters tab
            app.WallDistanceLabel = uilabel(app.ParametersTab);
            app.WallDistanceLabel.Text = 'Distance to Wall (cm):';
            app.WallDistanceLabel.Position = [450 420 120 22];
            
            app.WallDistanceEditField = uieditfield(app.ParametersTab, 'numeric');
            app.WallDistanceEditField.Position = [570 420 100 22];
            app.WallDistanceEditField.Value = 1;

            app.MaxSpacingLabel = uilabel(app.ParametersTab);
            app.MaxSpacingLabel.Text = 'Max spacing (deg):';
            app.MaxSpacingLabel.Position = [450 380 120 22];

            app.MaxSpacingEditField = uieditfield(app.ParametersTab, 'numeric');
            app.MaxSpacingEditField.Position = [570 380 100 22];
            app.MaxSpacingEditField.Value = 1;
            
            app.LoopCountLabel = uilabel(app.ParametersTab);
            app.LoopCountLabel.Text = 'Loop Count:';
            app.LoopCountLabel.Position = [450 340 120 22];
            
            app.LoopCountEditField = uieditfield(app.ParametersTab, 'numeric');
            app.LoopCountEditField.Position = [570 340 100 22];
            app.LoopCountEditField.Value = 1;
        
            app.IntervalLabel = uilabel(app.ParametersTab);
            app.IntervalLabel.Text = 'Interval (ms):';
            app.IntervalLabel.Position = [450 300 120 22];
            
            app.IntervalEditField = uieditfield(app.ParametersTab, 'numeric');
            app.IntervalEditField.Position = [570 300 100 22];
            app.IntervalEditField.Value = 20; % Default value
        
            app.XOffsetLabel = uilabel(app.ParametersTab);
            app.XOffsetLabel.Text = 'X-Offset (cm):';
            app.XOffsetLabel.Position = [450 260 100 22];
            
            app.XOffsetEditField = uieditfield(app.ParametersTab, 'numeric');
            app.XOffsetEditField.Position = [570 260 100 22];
            app.XOffsetEditField.Value = 0;
        
            app.YOffsetLabel = uilabel(app.ParametersTab);
            app.YOffsetLabel.Text = 'Y-Offset (cm):';
            app.YOffsetLabel.Position = [450 220 100 22];
            
            app.YOffsetEditField = uieditfield(app.ParametersTab, 'numeric');
            app.YOffsetEditField.Position = [570 220 100 22];
            app.YOffsetEditField.Value = 0;
        
            % Create buttons for action in the Parameters tab
            app.CreateButton = uibutton(app.ParametersTab, 'push');
            app.CreateButton.ButtonPushedFcn = createCallbackFcn(app, @createButtonPushed, true);
            app.CreateButton.Position = [450 180 100 22];
            app.CreateButton.Text = 'Create shape';
            
            app.ExecuteButton = uibutton(app.ParametersTab, 'push');
            app.ExecuteButton.ButtonPushedFcn = createCallbackFcn(app, @executeButtonPushed, true);
            app.ExecuteButton.Position = [570 180 100 22];
            app.ExecuteButton.Text = 'Create command';

            % Create the mode switch in the Parameters tab
            app.ModeSwitch = uiswitch(app.ParametersTab, 'slider');
            app.ModeSwitch.Items = {'Coordinates', 'Angles'};
            app.ModeSwitch.Position = [540 140 100 22]; % Adjust the position and size as needed
            app.ModeSwitch.ValueChangedFcn = createCallbackFcn(app, @modeSwitchValue, true);
        
            % Create buttons for serial port control in the Serial tab
            app.OpenPortButton = uibutton(app.SerialTab, 'push');
            app.OpenPortButton.ButtonPushedFcn = createCallbackFcn(app, @openPortButtonPushed, true);
            app.OpenPortButton.Position = [450 460 100 22];
            app.OpenPortButton.Text = 'Open Port';

            app.ClosePortButton = uibutton(app.SerialTab, 'push');
            app.ClosePortButton.ButtonPushedFcn = createCallbackFcn(app, @closePortButtonPushed, true);
            app.ClosePortButton.Position = [560 460 100 22];
            app.ClosePortButton.Text = 'Close Port';
        
            % Create button for sending serial message in the Serial tab
            app.SendButton = uibutton(app.SerialTab, 'push');
            app.SendButton.ButtonPushedFcn = createCallbackFcn(app, @sendSerialMessage, true);
            app.SendButton.Position = [510 410 100 22];
            app.SendButton.Text = 'Send';
        
            % Create dropdown menu for selecting serial port in the Serial tab
            app.SerialPortDropDown = uidropdown(app.SerialTab);
            app.SerialPortDropDown.Items = serialportlist("available");
            app.SerialPortDropDown.Position = [450 300 200 22]; % Adjust position as needed

            % Create TextArea component for displaying messages
            app.MessageTextArea = uitextarea(app.UIFigure);
            app.MessageTextArea.Position = [20 20 400 50];  % Adjust position and size as needed
            app.MessageTextArea.Value = '';  % Initialize with empty text
        
            % Initially grey out fields
            app.SideLengthEditField.Visible = 'off';
            app.SquareSideLengthLabel.Visible = 'off';

            app.BaseLengthEditField.Visible = 'off';
            app.HeightEditField.Visible = 'off';
            app.TriangleBaseLengthLabel.Visible = 'off';
            app.TriangleHeightLabel.Visible = 'off';

            app.RadiusEditField.Visible = 'off';
            app.SetPointsEditField.Visible = 'off';
            app.CircleRadiusLabel.Visible = 'off';
            app.SetPointsLabel.Visible = 'off';
        
            startupFcn(app);
        end
    


    end

    % App creation and deletion
    methods (Access = public)

        % Construct app
        function app = ShapeApp
            % Create UIFigure and components
            createComponents(app)

            app.Mode = 'coordinates';

            % Register the app with App Designer
            registerApp(app, app.UIFigure)

            if nargout == 0
                clear app
            end
        end

        % Code that executes before app deletion
        function delete(app)
            % Delete UIFigure when app is deleted
            disp('Shutting down application...');
            pause(1);
            app.closed = true;
            pause(1);
            delete(app.UIFigure)
        end
    end
end
