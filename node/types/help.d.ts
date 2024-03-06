import { Feature } from "./feature";
declare class DefaultHelpFeature extends Feature {
    constructor();
    complete: (editing: boolean, args: string[]) => string[];
    entry(...args: string[]): number;
    private showBriefHelp;
    private showOptionsHelp;
    private showFeatureHelp;
    private showModuleHelp;
}
export declare const defaultHelpFeature: DefaultHelpFeature;
export {};
